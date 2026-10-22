#include "mainwindow.h"
#include "settings.h"
#include "styles.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QPainter>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QToolButton>
#include <QFileIconProvider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(1190, 750);
    setStyleSheet(MAIN_WINDOW_STYLE);
    ui->setupUi(this);
//Windows:
    m_gamelist = new gamelistWindow(this);
    m_gamelist->setGeometry(232, 0, 958, 750);
    m_gamelist->show();

    m_optimizationWindow = new optimizationWindow(this);
    m_optimizationWindow->setGeometry(232, 0, 958, 750);
    m_optimizationWindow ->hide();

    m_settings = new Settings(this);
    m_settings->setGeometry(232, 0, 958, 750);
    m_settings ->hide();
//Line:
    QFrame *line = new QFrame(this);
    line->setGeometry(232, 0, 1, 750);
    line->setStyleSheet("background-color: black;");
    line->show();
    line->raise();
//Buttons:
    QPushButton *gamelistButton = new QPushButton("Gamelist", this);
    gamelistButton->setGeometry(16, 16, 200, 80);
    gamelistButton->show();
    gamelistButton->setStyleSheet(MENU_CARD_STYLE);
    connect(gamelistButton, &QPushButton::clicked, this, &MainWindow::on_gamelistButton_clicked);

    QPushButton *optimizationWindowButton = new QPushButton("Optimization", this);
    optimizationWindowButton->setGeometry(16, 112, 200, 80);
    optimizationWindowButton->show();
    optimizationWindowButton->setStyleSheet(MENU_CARD_STYLE);
    connect(optimizationWindowButton, &QPushButton::clicked, this, &MainWindow::on_optimizationWindowButton_clicked);

    QPushButton *SettingsButton = new QPushButton("Settings", this);
    SettingsButton->setGeometry(16, 204, 200, 80);
    SettingsButton->show();
    SettingsButton->setStyleSheet(MENU_CARD_STYLE);
    connect(SettingsButton, &QPushButton::clicked, this, &MainWindow::on_settingsbutton_clicked);
    connect(m_settings, &Settings::changeTheme, this, &MainWindow::changeTheme);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_gamelistButton_clicked() {
    m_settings->hide();
    m_optimizationWindow->hide();
    m_gamelist->show();
    m_gamelist->raise();
    openFileButton->show();
    openFileButton->raise();
}

void MainWindow::on_optimizationWindowButton_clicked() {
    m_gamelist->hide();
    m_settings->hide();
    m_optimizationWindow->show();
    m_optimizationWindow->raise();
}

void MainWindow::on_settingsbutton_clicked() {
    m_gamelist->hide();
    m_optimizationWindow->hide();
    m_settings->show();
    m_settings->raise();
}

void MainWindow::changeTheme() {

}













