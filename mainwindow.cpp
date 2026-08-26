#include "mainwindow.h"
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
    ui->setupUi(this);
//Windows:
    m_gamelist = new gamelistWindow(this);
    m_gamelist->setGeometry(232, 0, 958, 750);
    m_gamelist->show();

    m_optimizationWindow = new optimizationWindow(this);
    m_optimizationWindow->setGeometry(232, 0, 958, 750);
    m_optimizationWindow ->hide();
//Buttons:
    QPushButton *gamelistButton = new QPushButton("gamelist", this);
    gamelistButton->setGeometry(16, 16, 200, 80);
    gamelistButton->show();
    gamelistButton->setStyleSheet(MENU_CARD_STYLE);
    connect(gamelistButton, &QPushButton::clicked, this, &MainWindow::on_gamelistButton_clicked);

    QPushButton *optimizationWindowButton = new QPushButton("optimization", this);
    optimizationWindowButton->setGeometry(16, 112, 200, 80);
    optimizationWindowButton->show();
    optimizationWindowButton->setStyleSheet(MENU_CARD_STYLE);
    connect(optimizationWindowButton, &QPushButton::clicked, this, &MainWindow::on_optimizationWindowButton_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    drawLine(painter);
}

void MainWindow::drawLine(QPainter &painter){
    painter.drawLine(232, 0, 232, 750);
}

void MainWindow::on_gamelistButton_clicked() {
    m_gamelist->show();
    m_optimizationWindow->hide();
}

void MainWindow::on_optimizationWindowButton_clicked() {
    m_gamelist->hide();
    m_optimizationWindow->show();
}















