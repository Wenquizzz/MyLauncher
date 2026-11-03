#define NOMINMAX
#include <windows.h>
#include "steamscanner.h"
#include <filesystem>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <map>
#include <set>
#include <QDebug>

#include "vdf_parser.h"

namespace fs = std::filesystem;

// ================================================================ Utilities

static std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return (char)std::tolower(c); });
    return s;
}

static std::string ToUtf8(const std::wstring& w) {
    if (w.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(),
                                nullptr, 0, nullptr, nullptr);
    std::string r(n, 0);
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(),
                        r.data(), n, nullptr, nullptr);
    return r;
}

static std::string PathToUtf8(const fs::path& p) {
    return ToUtf8(p.wstring());
}

// ================================================================ Registry

static std::wstring ReadRegistryString(HKEY root, const std::wstring& subKey,
                                       const std::wstring& valueName) {
    HKEY hKey;
    if (RegOpenKeyExW(root, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return L"";

    wchar_t buffer[1024];
    DWORD bufferSize = sizeof(buffer);
    DWORD type = 0;
    LSTATUS st = RegQueryValueExW(hKey, valueName.c_str(), nullptr, &type,
                                  reinterpret_cast<LPBYTE>(buffer), &bufferSize);
    RegCloseKey(hKey);

    if (st == ERROR_SUCCESS && type == REG_SZ)
        return std::wstring(buffer);
    return L"";
}

static fs::path GetSteamInstallPath() {
    std::wstring p = ReadRegistryString(
        HKEY_CURRENT_USER, L"Software\\Valve\\Steam", L"SteamPath");
    if (p.empty()) {
        p = ReadRegistryString(
            HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Valve\\Steam",
            L"InstallPath");
    }
    if (!p.empty()) return fs::path(p);
    return {};
}

// ================================================================ Libraries

static std::string PathKey(const fs::path& p) {
    std::error_code ec;
    fs::path norm = fs::weakly_canonical(p, ec);
    if (ec) norm = p;
    std::string s = norm.string();
    std::replace(s.begin(), s.end(), '/', '\\');
    return ToLower(s);
}

static std::vector<fs::path> FindSteamLibraries(const fs::path& steamPath) {
    std::vector<fs::path> libs;
    std::set<std::string> seen;

    auto addIfNew = [&](const fs::path& p) {
        std::string k = PathKey(p);
        if (k.empty()) return;
        if (seen.insert(k).second) libs.push_back(p);
    };

    addIfNew(steamPath);

    fs::path vdf = steamPath / "steamapps" / "libraryfolders.vdf";
    if (fs::exists(vdf)) {
        try {
            std::ifstream f(vdf);
            auto root = tyti::vdf::read(f);
            for (const auto& c : root.childs) {
                auto it = c.second->attribs.find("path");
                if (it != c.second->attribs.end())
                    addIfNew(fs::path(it->second));
            }
        }
        catch (const std::exception& e) {
            qDebug() << "Error reading libraryfolders.vdf:" << e.what();
        }
    }
    return libs;
}

// ================================================================ GameInfo

struct GameInfo {
    std::string appId;
    std::string name;
    fs::path    installDir;
    fs::path    exePath;
    int         stateFlags = -1;
};

// ================================================================ Junk filters

static bool IsJunkExe(const std::string& lower) {
    static const char* junk[] = {
        "unins", "uninstall", "setup", "install", "installer",
        "crashhandler", "crashreport", "crashpad", "bugreport", "bugsplat",
        "vcredist", "vc_redist", "dxsetup", "dxwebsetup", "directx",
        "dotnet", "oalinst", "physx", "prereq", "ue4prereqsetup",
        "ue5prereqsetup", "easyanticheat", "eac_", "battleye",
        "eula", "readme", "manual", "license",
        "helper", "updater", "updcfg", "update",
        "steamerrorreporter", "touchup", "activation",
        "unitycrashhandler",
        "benchmark", "benchmarktool", "config", "settings", "diagnostics",
        "buildstaticplevels", "converter", "importer", "exporttool",
        "modtool", "modding", "devtool",
        "srcds", "hlds", "dedicatedserver", "dedicated_server", "gameserver"
    };
    for (const char* j : junk)
        if (lower.find(j) != std::string::npos) return true;
    return false;
}

static bool IsJunkDir(const std::string& lower) {
    static const char* junk[] = {
        "_commonredist", "commonredist", "redist", "redistributables",
        "installer", "installers", "directx", "dotnet", "vcredist",
        "support", "docs", "documentation",
        "tools", "sdk", "editor", "modding", "modtools",
        "backup", "cache", "temp",
        "test", "tests", "unittests",
        "samples", "examples",
        "dedicatedserver", "server",
        "__macosx", ".git", ".svn"
    };
    for (const char* j : junk)
        if (lower == j) return true;
    return false;
}

static bool IsPreferredDir(const std::string& lower) {
    static const char* pref[] = {
        "bin", "binaries", "win64", "win32", "x64", "x86",
        "bin64", "bin32", "release", "retail", "system",
        "game", "app", "runtime"
    };
    for (const char* p : pref)
        if (lower.find(p) != std::string::npos) return true;
    return false;
}

// ================================================================ EXE search

static fs::path FindExeFilesystem(const fs::path& installDir,
                                  const std::string& gameName,
                                  const std::string& installDirName,
                                  int maxDepth = 5) {
    if (!fs::exists(installDir) || !fs::is_directory(installDir)) return {};

    struct Cand { fs::path p; uintmax_t size; int depth; int score; };
    std::vector<Cand> cands;

    auto norm = [](const std::string& s) {
        std::string r;
        for (char c : s)
            if (std::isalnum((unsigned char)c))
                r.push_back((char)std::tolower((unsigned char)c));
        return r;
    };
    std::string nGame = norm(gameName);
    std::string nDir = norm(installDirName);

    std::error_code ec;
    fs::recursive_directory_iterator it(
        installDir, fs::directory_options::skip_permission_denied, ec);
    fs::recursive_directory_iterator end;

    for (; it != end; it.increment(ec)) {
        if (ec) { ec.clear(); continue; }
        if (it.depth() > maxDepth) { it.disable_recursion_pending(); continue; }

        if (it->is_directory(ec)) {
            std::string d = ToLower(it->path().filename().string());
            if (IsJunkDir(d)) it.disable_recursion_pending();
            continue;
        }
        if (!it->is_regular_file(ec)) continue;
        if (ToLower(it->path().extension().string()) != ".exe") continue;
        std::string fnLower = ToLower(it->path().filename().string());
        if (IsJunkExe(fnLower)) continue;

        uintmax_t sz = fs::file_size(it->path(), ec);
        if (ec) sz = 0;

        std::string stem = norm(it->path().stem().string());
        int score = 0;

        if (!nDir.empty() && stem == nDir)  score += 150;
        if (!nGame.empty() && stem == nGame) score += 100;
        else if (!nGame.empty() && !stem.empty() &&
                 (nGame.find(stem) != std::string::npos ||
                  stem.find(nGame) != std::string::npos)) score += 60;

        std::string parentLower = ToLower(it->path().parent_path().string());
        if (IsPreferredDir(parentLower)) score += 40;

        score -= it.depth() * 5;

        int mb = (int)(sz / (1024 * 1024));
        score += (std::min)(mb, 50);

        if (sz < 100 * 1024) score -= 30;

        if (fnLower.find("launcher") != std::string::npos) score -= 20;

        cands.push_back({ it->path(), sz, it.depth(), score });
    }

    if (cands.empty()) return {};

    std::sort(cands.begin(), cands.end(), [](const Cand& a, const Cand& b) {
        if (a.score != b.score) return a.score > b.score;
        if (a.size != b.size)  return a.size > b.size;
        return a.depth < b.depth;
    });
    return cands.front().p;
}

// ================================================================ Library scan

static std::vector<GameInfo> ScanLibrary(const fs::path& libraryPath) {
    std::vector<GameInfo> games;
    fs::path steamApps = libraryPath / "steamapps";
    if (!fs::exists(steamApps)) return games;

    std::regex reAppId(R"REGEX("appid"\s+"([^"]+)")REGEX");
    std::regex reName(R"REGEX("name"\s+"([^"]+)")REGEX");
    std::regex reInstallDir(R"REGEX("installdir"\s+"([^"]+)")REGEX");
    std::regex reStateFlags(R"REGEX("StateFlags"\s+"(\d+)")REGEX");

    for (const auto& entry : fs::directory_iterator(steamApps)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".acf")
            continue;
        std::string fn = entry.path().filename().string();
        if (fn.rfind("appmanifest_", 0) != 0) continue;

        try {
            std::ifstream f(entry.path());
            if (!f.is_open()) continue;
            std::string content((std::istreambuf_iterator<char>(f)),
                                std::istreambuf_iterator<char>());

            GameInfo info;
            std::smatch m;

            if (std::regex_search(content, m, reAppId) && m.size() > 1)
                info.appId = m[1].str();
            if (std::regex_search(content, m, reName) && m.size() > 1)
                info.name = m[1].str();

            std::string installName;
            if (std::regex_search(content, m, reInstallDir) && m.size() > 1) {
                installName = m[1].str();
                info.installDir = libraryPath / "steamapps" / "common" / installName;
            }

            if (std::regex_search(content, m, reStateFlags) && m.size() > 1) {
                try { info.stateFlags = std::stoi(m[1].str()); }
                catch (...) { info.stateFlags = -1; }
            }

            if (!info.appId.empty() && !info.name.empty())
                games.push_back(std::move(info));
        }
        catch (const std::exception& e) {
            qDebug() << "Cannot read" << QString::fromStdString(PathToUtf8(entry.path())) << ":" << e.what();
        }
    }
    return games;
}

// ================================================================ Steam infra filter

static bool IsSteamInfrastructure(const std::string& appId) {
    static const std::set<std::string> ids = {
        "228980", "1070560", "1391110", "1628350", "4183110",
        "1493710", "961940", "2348590", "2805730", "3658110",
        "1826330", "1161040", "250820", "858280", "1887720"
    };
    return ids.count(appId) > 0;
}

// ================================================================ Main Scan Function

QVector<AppItem> SteamScanner::scan() {
    QVector<AppItem> result;

    fs::path steamPath = GetSteamInstallPath();
    if (steamPath.empty()) {
        qDebug() << "Cannot find Steam install path";
        return result;
    }

    auto libs = FindSteamLibraries(steamPath);

    std::map<std::string, GameInfo> byId;
    for (const auto& lib : libs) {
        for (auto& g : ScanLibrary(lib)) {
            auto it = byId.find(g.appId);
            if (it == byId.end()) {
                byId[g.appId] = std::move(g);
            }
            else if (it->second.installDir.empty() || !fs::exists(it->second.installDir)) {
                if (fs::exists(g.installDir))
                    it->second = std::move(g);
            }
        }
    }

    for (auto& [id, g] : byId) {
        if (IsSteamInfrastructure(id)) continue;

        std::string installName = g.installDir.filename().string();
        fs::path exe = FindExeFilesystem(g.installDir, g.name, installName);

        if (!exe.empty()) {
            AppItem item;
            item.name = QString::fromStdString(g.name);
            item.exePath = QString::fromStdString(PathToUtf8(exe));

            result.append(item);
            qDebug() << "Found Steam Game:" << item.name << "->" << item.exePath;
        } else {
            qDebug() << "Could not find exe for:" << QString::fromStdString(g.name);
        }
    }

    return result;
}