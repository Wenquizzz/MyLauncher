#include "optimizationwindow.h"
#include "styles.h"
#include <QBoxLayout>
#include <QTabWidget>
#include <QProcess>
#include <QCheckBox>
#include <QSettings>

optimizationWindow::optimizationWindow(QWidget *parent)
    : QWidget(parent)
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
    connect(powerPlanCheckBox, &QCheckBox::toggled, this, [powerPlanCheckBox]() {
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
    connect(gameModeCheckBox, &QCheckBox::toggled, this, [gameModeCheckBox]() {
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
    connect(visualEffectsCheckBox, &QCheckBox::toggled, this, [visualEffectsCheckBox]() {
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
    connect(backgroundAppsCheckBox, &QCheckBox::toggled, this, [backgroundAppsCheckBox]() {
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
    connect(hagsCheckBox, &QCheckBox::toggled, this, [hagsCheckBox]() {
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
    connect(networkThrottlingCheckBox, &QCheckBox::toggled, this, [networkThrottlingCheckBox]() {
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
    connect(telemetryCheckBox, &QCheckBox::toggled, this, [telemetryCheckBox]() {
        QSettings s("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection", QSettings::NativeFormat);
        if (telemetryCheckBox->isChecked()) {
            s.setValue("AllowTelemetry", 0);
        } else {
            s.setValue("AllowTelemetry", 1);
        }
    });
    //Disable Memory Compression
    QCheckBox *memCompressionCheckBox = new QCheckBox("Disable Memory Compression", systemPage);
    memCompressionCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(memCompressionCheckBox);
    connect(memCompressionCheckBox, &QCheckBox::toggled, this, [memCompressionCheckBox]() {
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
    connect(storageSenseCheckBox, &QCheckBox::toggled, this, [storageSenseCheckBox]() {
        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\StorageSense\\Parameters\\StoragePolicy", QSettings::NativeFormat);
        if (storageSenseCheckBox->isChecked()) {
            s.setValue("0", 1);
        } else {
            s.setValue("0", 0);
        }
    });

    QCheckBox *gameDvrCheckBox = new QCheckBox("Disable Game DVR (Background Recording)", systemPage);
    gameDvrCheckBox->setStyleSheet(CHECKBOX_STYLE);
    systemLayout->addWidget(gameDvrCheckBox);
    connect(gameDvrCheckBox, &QCheckBox::toggled, this, [gameDvrCheckBox]() {
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

    //"Disable Fullscreen Optimizations
    QCheckBox *fsoCheckBox = new QCheckBox("Disable Fullscreen Optimizations", Intelpage);
    fsoCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(fsoCheckBox);
    connect(fsoCheckBox, &QCheckBox::toggled, this, [fsoCheckBox]() {
        QSettings s("HKEY_CURRENT_USER\\System\\GameConfigStore", QSettings::NativeFormat);
        if (fsoCheckBox->isChecked()) {
            s.setValue("GameDVR_FSEBehaviorMode", 2);
            s.setValue("GameDVR_HonorUserFSEBehaviorMode", 1);
        } else {
            s.setValue("GameDVR_FSEBehaviorMode", 0);
            s.setValue("GameDVR_HonorUserFSEBehaviorMode", 0);
        }
    });

    //Disable Virtualization-Based Security
    QCheckBox *vbsCheckBox = new QCheckBox("Disable Virtualization-Based Security", Intelpage);
    vbsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(vbsCheckBox);
    connect(vbsCheckBox, &QCheckBox::toggled, this, [vbsCheckBox]() {
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

    //Optimize Intel Thread Priority
    QCheckBox *intelPriorityCheckBox = new QCheckBox("Optimize Intel Thread Priority for Active Game", Intelpage);
    intelPriorityCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(intelPriorityCheckBox);
    connect(intelPriorityCheckBox, &QCheckBox::toggled, this, [intelPriorityCheckBox]() {
        QSettings s("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\PriorityControl", QSettings::NativeFormat);
        if (intelPriorityCheckBox->isChecked()) {
            s.setValue("Win32PrioritySeparation", 38);
        } else {
            s.setValue("Win32PrioritySeparation", 2);
        }
    });

    //Disable Intel Core Parking
    QCheckBox *unparkCheckBox = new QCheckBox("Disable Intel Core Parking", Intelpage);
    unparkCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(unparkCheckBox);
    connect(unparkCheckBox, &QCheckBox::toggled, this, [unparkCheckBox]() {
        QProcess process;
        if (unparkCheckBox->isChecked()) {
            process.start("cmd", QStringList() << "/c" << "powercfg -setacvalueindex scheme_current sub_processor cppa87635-130d-478c-bc5e-7210785f7e27 100 && powercfg -setactive scheme_current");
        } else {
            process.start("cmd", QStringList() << "/c" << "powercfg -setacvalueindex scheme_current sub_processor cppa87635-130d-478c-bc5e-7210785f7e27 0 && powercfg -setactive scheme_current");
        }
        process.waitForFinished();
    });

    //Intel Network Latency Optimization
    QCheckBox *intelNetworkCheckBox = new QCheckBox("Intel Network Latency Optimization", Intelpage);
    intelNetworkCheckBox->setStyleSheet(CHECKBOX_STYLE);
    IntelLayout->addWidget(intelNetworkCheckBox);
    connect(intelNetworkCheckBox, &QCheckBox::toggled, this, [intelNetworkCheckBox]() {
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