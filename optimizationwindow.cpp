#include "optimizationwindow.h"
#include "styles.h"
#include "database.h"
#include <QBoxLayout>
#include <QTabWidget>
#include <QProcess>
#include <QCheckBox>
#include <QSettings>

optimizationWindow::optimizationWindow(Database *db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setGeometry(0, 0, 958, 750);
    QTabWidget *tabs = new QTabWidget(this);
    tabs->setStyleSheet(QTAB_STYLE);
    tabs->setGeometry(0, 0, 957, 750);
    QWidget *systemPage = new QWidget();
    QWidget *Intelpage = new QWidget();
    QVBoxLayout *systemLayout = new QVBoxLayout(systemPage);
    QVBoxLayout *IntelLayout = new QVBoxLayout(Intelpage);
    systemLayout->setContentsMargins(32, 32, 32, 32);
    systemLayout->setSpacing(30);
    IntelLayout->setContentsMargins(32, 32, 32, 32);
    IntelLayout->setSpacing(30);

    //System:

    // Power plan
    QCheckBox *powerPlanCheckBox = new QCheckBox("High performance power plan", systemPage);
    powerPlanCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(powerPlanCheckBox);
    int statePowerPlan = m_db->getTweakState("powerPlan");
    if (statePowerPlan == 1) {
        powerPlanCheckBox->setChecked(true);
    } else if (statePowerPlan == 0) {
        powerPlanCheckBox->setChecked(false);
    }
    connect(powerPlanCheckBox, &QCheckBox::toggled, this, [this, powerPlanCheckBox]() {
        int newState = 0;
        if (powerPlanCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("powerPlan", newState);

        QProcess process;
        if (powerPlanCheckBox->isChecked()) {
            process.start("powercfg", QStringList() << "/setactive" << "SCHEME_MIN");
        } else {
            process.start("powercfg", QStringList() << "/setactive" << "SCHEME_BALANCED");
        }
        process.waitForFinished();
    });

    // Gamemode
    QCheckBox *gameModeCheckBox = new QCheckBox("Gamemode", systemPage);
    gameModeCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(gameModeCheckBox);
    int stateGameMode = m_db->getTweakState("gameMode");
    if (stateGameMode == 1) {
        gameModeCheckBox->setChecked(true);
    } else if (stateGameMode == 0) {
        gameModeCheckBox->setChecked(false);
    }
    connect(gameModeCheckBox, &QCheckBox::toggled, this, [this, gameModeCheckBox]() {
        int newState = 0;
        if (gameModeCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("gameMode", newState);

        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\GameBar", QSettings::NativeFormat);
        if (gameModeCheckBox->isChecked()) {
            s.setValue("AutoGameModeEnabled", 1);
            s.setValue("AllowAutoGameMode", 1);
        } else {
            s.setValue("AutoGameModeEnabled", 0);
            s.setValue("AllowAutoGameMode", 0);
        }
    });

    // Visual effects
    QCheckBox *visualEffectsCheckBox = new QCheckBox("Windows virtual effects off", systemPage);
    visualEffectsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(visualEffectsCheckBox);
    int stateVisual = m_db->getTweakState("visualEffects");
    if (stateVisual == 1) {
        visualEffectsCheckBox->setChecked(true);
    } else if (stateVisual == 0) {
        visualEffectsCheckBox->setChecked(false);
    }
    connect(visualEffectsCheckBox, &QCheckBox::toggled, this, [this, visualEffectsCheckBox]() {
        int newState = 0;
        if (visualEffectsCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("visualEffects", newState);

        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects", QSettings::NativeFormat);
        if (visualEffectsCheckBox->isChecked()) {
            s.setValue("VisualFXSetting", 2);
        } else {
            s.setValue("VisualFXSetting", 1);
        }
    });

    // Background apps
    QCheckBox *backgroundAppsCheckBox = new QCheckBox("Disable background apps", systemPage);
    backgroundAppsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(backgroundAppsCheckBox);
    int stateBgApps = m_db->getTweakState("backgroundApps");
    if (stateBgApps == 1) {
        backgroundAppsCheckBox->setChecked(true);
    } else if (stateBgApps == 0) {
        backgroundAppsCheckBox->setChecked(false);
    }
    connect(backgroundAppsCheckBox, &QCheckBox::toggled, this, [this, backgroundAppsCheckBox]() {
        int newState = 0;
        if (backgroundAppsCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("backgroundApps", newState);

        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", QSettings::NativeFormat);
        if (backgroundAppsCheckBox->isChecked()) {
            s.setValue("GlobalUserDisabled", 1);
        } else {
            s.setValue("GlobalUserDisabled", 0);
        }
    });

    // Hardware GPU Scheduling (HAGS)
    QCheckBox *hagsCheckBox = new QCheckBox("Hardware GPU Scheduling (HAGS)", systemPage);
    hagsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(hagsCheckBox);
    int stateHags = m_db->getTweakState("hags");
    if (stateHags == 1) {
        hagsCheckBox->setChecked(true);
    } else if (stateHags == 0) {
        hagsCheckBox->setChecked(false);
    }
    connect(hagsCheckBox, &QCheckBox::toggled, this, [this, hagsCheckBox]() {
        int newState = 0;
        if (hagsCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("hags", newState);

        QSettings s("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers", QSettings::NativeFormat);
        if (hagsCheckBox->isChecked()) {
            s.setValue("HwSchMode", 2);
        } else {
            s.setValue("HwSchMode", 1);
        }
    });

    // Network Throttling
    QCheckBox *networkThrottlingCheckBox = new QCheckBox("Network speed limiting disable", systemPage);
    networkThrottlingCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(networkThrottlingCheckBox);
    int stateNetThrot = m_db->getTweakState("networkThrottling");
    if (stateNetThrot == 1) {
        networkThrottlingCheckBox->setChecked(true);
    } else if (stateNetThrot == 0) {
        networkThrottlingCheckBox->setChecked(false);
    }
    connect(networkThrottlingCheckBox, &QCheckBox::toggled, this, [this, networkThrottlingCheckBox]() {
        int newState = 0;
        if (networkThrottlingCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("networkThrottling", newState);

        QSettings s("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", QSettings::NativeFormat);
        if (networkThrottlingCheckBox->isChecked()) {
            s.setValue("NetworkThrottlingIndex", 0xffffffff);
        } else {
            s.setValue("NetworkThrottlingIndex", 0xa);
        }
    });

    // Telemetry
    QCheckBox *telemetryCheckBox = new QCheckBox("Disable telemetry", systemPage);
    telemetryCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(telemetryCheckBox);
    int stateTelemetry = m_db->getTweakState("telemetry");
    if (stateTelemetry == 1) {
        telemetryCheckBox->setChecked(true);
    } else if (stateTelemetry == 0) {
        telemetryCheckBox->setChecked(false);
    }
    connect(telemetryCheckBox, &QCheckBox::toggled, this, [this, telemetryCheckBox]() {
        int newState = 0;
        if (telemetryCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("telemetry", newState);

        QSettings s("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection", QSettings::NativeFormat);
        if (telemetryCheckBox->isChecked()) {
            s.setValue("AllowTelemetry", 0);
        } else {
            s.setValue("AllowTelemetry", 1);
        }
    });

    // Disable Memory Compression
    QCheckBox *memCompressionCheckBox = new QCheckBox("Disable Memory Compression", systemPage);
    memCompressionCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(memCompressionCheckBox);
    int stateMemComp = m_db->getTweakState("memCompression");
    if (stateMemComp == 1) {
        memCompressionCheckBox->setChecked(true);
    } else if (stateMemComp == 0) {
        memCompressionCheckBox->setChecked(false);
    }
    connect(memCompressionCheckBox, &QCheckBox::toggled, this, [this, memCompressionCheckBox]() {
        int newState = 0;
        if (memCompressionCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("memCompression", newState);

        QProcess process;
        if (memCompressionCheckBox->isChecked()) {
            process.start("powershell", QStringList() << "-Command" << "Disable-MMAgent -MemoryCompression");
        } else {
            process.start("powershell", QStringList() << "-Command" << "Enable-MMAgent -MemoryCompression");
        }
        process.waitForFinished();
    });

    // Storage Sense (Auto-clean temp files)
    QCheckBox *storageSenseCheckBox = new QCheckBox("Enable Storage Sense (Auto-clean)", systemPage);
    storageSenseCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(storageSenseCheckBox);
    int stateStorage = m_db->getTweakState("storageSense");
    if (stateStorage == 1) {
        storageSenseCheckBox->setChecked(true);
    } else if (stateStorage == 0) {
        storageSenseCheckBox->setChecked(false);
    }
    connect(storageSenseCheckBox, &QCheckBox::toggled, this, [this, storageSenseCheckBox]() {
        int newState = 0;
        if (storageSenseCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("storageSense", newState);

        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\StorageSense\\Parameters\\StoragePolicy", QSettings::NativeFormat);
        if (storageSenseCheckBox->isChecked()) {
            s.setValue("0", 1);
        } else {
            s.setValue("0", 0);
        }
    });

    // Disable Game DVR
    QCheckBox *gameDvrCheckBox = new QCheckBox("Disable Game DVR (Background Recording)", systemPage);
    gameDvrCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(gameDvrCheckBox);
    int stateGameDvr = m_db->getTweakState("gameDvr");
    if (stateGameDvr == 1) {
        gameDvrCheckBox->setChecked(true);
    } else if (stateGameDvr == 0) {
        gameDvrCheckBox->setChecked(false);
    }
    connect(gameDvrCheckBox, &QCheckBox::toggled, this, [this, gameDvrCheckBox]() {
        int newState = 0;
        if (gameDvrCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("gameDvr", newState);

        QProcess process;
        if (gameDvrCheckBox->isChecked()) {
            process.start("cmd", QStringList() << "/c"
                                               << "reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\GameDVR\" /v AppCaptureEnabled /t REG_DWORD /d 0 /f && "
                                               << "reg add \"HKCU\\System\\GameConfigStore\" /v GameDVR_Enabled /t REG_DWORD /d 0 /f");
        } else {
            process.start("cmd", QStringList()
                          << "/c"
                          << "reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\GameDVR\" /v AppCaptureEnabled /t REG_DWORD /d 1 /f && "
                          << "reg add \"HKCU\\System\\GameConfigStore\" /v GameDVR_Enabled /t REG_DWORD /d 1 /f");
        }
        process.waitForFinished();
    });

    systemLayout->addStretch();
    tabs->addTab(systemPage, "System");

    //Intel:

    // Disable Fullscreen Optimizations
    QCheckBox *fsoCheckBox = new QCheckBox("Disable Fullscreen Optimizations", Intelpage);
    fsoCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(fsoCheckBox);
    int stateFso = m_db->getTweakState("fso");
    if (stateFso == 1) {
        fsoCheckBox->setChecked(true);
    } else if (stateFso == 0) {
        fsoCheckBox->setChecked(false);
    }
    connect(fsoCheckBox, &QCheckBox::toggled, this, [this, fsoCheckBox]() {
        int newState = 0;
        if (fsoCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("fso", newState);

        QSettings s("HKEY_CURRENT_USER\\System\\GameConfigStore", QSettings::NativeFormat);
        if (fsoCheckBox->isChecked()) {
            s.setValue("GameDVR_FSEBehaviorMode", 2);
            s.setValue("GameDVR_HonorUserFSEBehaviorMode", 1);
        } else {
            s.setValue("GameDVR_FSEBehaviorMode", 0);
            s.setValue("GameDVR_HonorUserFSEBehaviorMode", 0);
        }
    });

    // Disable Virtualization-Based Security
    QCheckBox *vbsCheckBox = new QCheckBox("Disable Virtualization-Based Security", Intelpage);
    vbsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(vbsCheckBox);
    int stateVbs = m_db->getTweakState("vbs");
    if (stateVbs == 1) {
        vbsCheckBox->setChecked(true);
    } else if (stateVbs == 0) {
        vbsCheckBox->setChecked(false);
    }
    connect(vbsCheckBox, &QCheckBox::toggled, this, [this, vbsCheckBox]() {
        int newState = 0;
        if (vbsCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("vbs", newState);

        QSettings s("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\DeviceGuard", QSettings::NativeFormat);
        QSettings sScenarios("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\DeviceGuard\\Scenarios\\HypervisorEnforcedCodeIntegrity", QSettings::NativeFormat);
        if (vbsCheckBox->isChecked()) {
            s.setValue("EnableVirtualizationBasedSecurity", 0);
            sScenarios.setValue("Enabled", 0);
        } else {
            s.setValue("EnableVirtualizationBasedSecurity", 1);
            sScenarios.setValue("Enabled", 1);
        }
    });

    // Optimize Intel Thread Priority
    QCheckBox *intelPriorityCheckBox = new QCheckBox("Optimize Intel Thread Priority for Active Game", Intelpage);
    intelPriorityCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(intelPriorityCheckBox);
    int stateIntPr = m_db->getTweakState("intelPriority");
    if (stateIntPr == 1) {
        intelPriorityCheckBox->setChecked(true);
    } else if (stateIntPr == 0) {
        intelPriorityCheckBox->setChecked(false);
    }
    connect(intelPriorityCheckBox, &QCheckBox::toggled, this, [this, intelPriorityCheckBox]() {
        int newState = 0;
        if (intelPriorityCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("intelPriority", newState);

        QSettings s("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\PriorityControl", QSettings::NativeFormat);
        if (intelPriorityCheckBox->isChecked()) {
            s.setValue("Win32PrioritySeparation", 38);
        } else {
            s.setValue("Win32PrioritySeparation", 2);
        }
    });

    // Disable Intel Core Parking
    QCheckBox *unparkCheckBox = new QCheckBox("Disable Intel Core Parking", Intelpage);
    unparkCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(unparkCheckBox);
    int stateUnpark = m_db->getTweakState("unpark");
    if (stateUnpark == 1) {
        unparkCheckBox->setChecked(true);
    } else if (stateUnpark == 0) {
        unparkCheckBox->setChecked(false);
    }
    connect(unparkCheckBox, &QCheckBox::toggled, this, [this, unparkCheckBox]() {
        int newState = 0;
        if (unparkCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("unpark", newState);

        QProcess process;
        if (unparkCheckBox->isChecked()) {
            process.start("cmd", QStringList() << "/c" << "powercfg -setacvalueindex scheme_current sub_processor cppa87635-130d-478c-bc5e-7210785f7e27 100 && powercfg -setactive scheme_current");
        } else {
            process.start("cmd", QStringList() << "/c" << "powercfg -setacvalueindex scheme_current sub_processor cppa87635-130d-478c-bc5e-7210785f7e27 0 && powercfg -setactive scheme_current");
        }
        process.waitForFinished();
    });

    // Intel Network Latency Optimization
    QCheckBox *intelNetworkCheckBox = new QCheckBox("Intel Network Latency Optimization", Intelpage);
    intelNetworkCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(intelNetworkCheckBox);
    int stateIntNet = m_db->getTweakState("intelNetwork");
    if (stateIntNet == 1) {
        intelNetworkCheckBox->setChecked(true);
    } else if (stateIntNet == 0) {
        intelNetworkCheckBox->setChecked(false);
    }
    connect(intelNetworkCheckBox, &QCheckBox::toggled, this, [this, intelNetworkCheckBox]() {
        int newState = 0;
        if (intelNetworkCheckBox->isChecked()) {
            newState = 1;
        }
        m_db->saveTweak("intelNetwork", newState);

        QProcess process;
        if (intelNetworkCheckBox->isChecked()) {
            process.start("netsh", QStringList() << "int" << "tcp" << "set" << "global" << "timestamps=disabled" << "autotuninglevel=normal" << "experimental=disabled");
        } else {
            process.start("netsh", QStringList() << "int" << "tcp" << "set" << "global" << "timestamps=allowed" << "autotuninglevel=normal" << "experimental=disabled");
        }
        process.waitForFinished();
    });

    IntelLayout->addStretch();
    tabs->addTab(Intelpage, "Intel");
}