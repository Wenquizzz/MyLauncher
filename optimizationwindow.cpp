#include "optimizationwindow.h"
#include "styles.h"
#include <QProcess>
#include <QCheckBox>
#include <QSettings>

optimizationWindow::optimizationWindow(QWidget *parent)
    : QWidget(parent)
{
//Powerplan:
    setGeometry(0, 0, 958, 750);
    QCheckBox *powerPlanCheckBox = new QCheckBox("High performance power plan", this);
    powerPlanCheckBox->setChecked(false);
    powerPlanCheckBox->setStyleSheet(CHECKBOX_STYLE);
    powerPlanCheckBox->setGeometry(32, 32, 500, 50);
    connect(powerPlanCheckBox, &QCheckBox::toggled, this, [powerPlanCheckBox]() {
        QProcess process;
        if (powerPlanCheckBox->isChecked()) {
            process.start("powercfg", QStringList() << "/setactive" << "SCHEME_MIN");
        } else {
            process.start("powercfg", QStringList() << "/setactive" << "SCHEME_BALANCED");
        }
        process.waitForFinished();
    });
//Gamemode:
    QCheckBox *gameModeCheckBox = new QCheckBox("Gamemode", this);
    gameModeCheckBox->setChecked(false);
    gameModeCheckBox->setStyleSheet(CHECKBOX_STYLE);
    gameModeCheckBox->setGeometry(32, 114, 250, 50);
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
//Windows virtual effects:
    QCheckBox *visualEffectsCheckBox = new QCheckBox("Windows virtual effects off", this);
    visualEffectsCheckBox->setChecked(false);
    visualEffectsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    visualEffectsCheckBox->setGeometry(32, 196, 400, 50);
    connect(visualEffectsCheckBox, &QCheckBox::toggled, this, [visualEffectsCheckBox]() {
        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects", QSettings::NativeFormat);
        if (visualEffectsCheckBox->isChecked()) {
            s.setValue("VisualFXSetting", 2);
        } else {
            s.setValue("VisualFXSetting", 1);
        }
    });
//Disable background apps
    QCheckBox *backgroundAppsCheckBox = new QCheckBox("Disable background apps", this);
    backgroundAppsCheckBox->setChecked(false);
    backgroundAppsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    backgroundAppsCheckBox->setGeometry(32, 278, 400, 50);
    connect(backgroundAppsCheckBox, &QCheckBox::toggled, this, [backgroundAppsCheckBox]() {
        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", QSettings::NativeFormat);
        if (backgroundAppsCheckBox->isChecked()) {
            s.setValue("GlobalUserDisabled", 1);
        } else {
            s.setValue("GlobalUserDisabled", 0);
        }
    });
//NetworkThrottling
    QCheckBox *hagsCheckBox = new QCheckBox("Network speed limiting disable", this);
    hagsCheckBox->setChecked(false);
    hagsCheckBox->setStyleSheet(CHECKBOX_STYLE);
    hagsCheckBox->setGeometry(32, 360, 400, 50);
    connect(hagsCheckBox, &QCheckBox::toggled, this, [hagsCheckBox]() {
        QSettings s("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers", QSettings::NativeFormat);
        if (hagsCheckBox->isChecked()) {
            s.setValue("NetworkThrottlingIndex", 0xffffffff);
        } else {
            s.setValue("NetworkThrottlingIndex", 0xa);
        }
    });
//Hardware GPU Scheduling
    QCheckBox *networkThrottlingCheckBox = new QCheckBox("Hardware GPU Scheduling", this);
    networkThrottlingCheckBox->setChecked(false);
    networkThrottlingCheckBox->setStyleSheet(CHECKBOX_STYLE);
    networkThrottlingCheckBox->setGeometry(32, 442, 400, 50);
    connect(networkThrottlingCheckBox, &QCheckBox::toggled, this, [networkThrottlingCheckBox]() {
        QSettings s("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", QSettings::NativeFormat);
        if (networkThrottlingCheckBox->isChecked()) {
            s.setValue("HwSchMode", 2);
        } else {
            s.setValue("HwSchMode", 1);
        }
    });
//Disable telemetry
    QCheckBox *telemetryCheckBox = new QCheckBox("Disable telemetry", this);
    telemetryCheckBox->setChecked(false);
    telemetryCheckBox->setStyleSheet(CHECKBOX_STYLE);
    telemetryCheckBox->setGeometry(32, 524, 400, 50);
    connect(telemetryCheckBox, &QCheckBox::toggled, this, [telemetryCheckBox]() {
        QSettings s("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection", QSettings::NativeFormat);
        if (telemetryCheckBox->isChecked()) {
            s.setValue("AllowTelemetry", 0);
        } else {
            s.setValue("AllowTelemetry", 1);
        }
    });
}
