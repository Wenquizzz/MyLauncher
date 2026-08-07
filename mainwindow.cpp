#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "styles.h"
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

    QPushButton *openFileButton = new QPushButton("Добавить файл", this);
    openFileButton ->setGeometry(16, 16, 200, 80);

    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::on_openFileButton_clicked);;
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

AppItem MainWindow::takeInfo() {
    QString filePath = QFileDialog::getOpenFileName(this, "Заголовок окна", "C:/", "Исполняемые файлы (*.exe)");
    AppItem data;
    if (filePath.isEmpty()) {
        return data;
    }
    data.name = QFileInfo(filePath).baseName();
    data.exePath = filePath;
    qDebug() << data.name;
    qDebug() << filePath;
    return data;
}

void MainWindow::setGeometryButton(QToolButton *opener){
    if (l>4) {
        l=0;
        h++;
    };
    int left_indent = 264+(l*182);
    int top_indent = 32+(h*242);
    opener ->setGeometry(left_indent, top_indent, 150, 210);
    l++;
}

void MainWindow::on_openFileButton_clicked() {
    m_currentApp = takeInfo();

    if (m_currentApp.exePath.isEmpty()) {
        return;
    }

    QToolButton *opener = new QToolButton(this);
    QFileIconProvider provider;
    QIcon icon = provider.icon(QFileInfo(m_currentApp.exePath));

    opener->setIcon(icon);
    opener->setIconSize(QSize(100, 100));
    opener->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    opener->setStyleSheet(GAME_CARD_STYLE);
    opener->setText(m_currentApp.name);
    setGeometryButton(opener);

    opener->setProperty("exePath", m_currentApp.exePath);
    opener-> show();

    connect(opener, &QPushButton::clicked, this, [opener]() {
        QString path = opener->property("exePath").toString();
        QProcess::startDetached(path);
    });
}

//void MainWindow::on_opener_clicked() {
//    QString path = opener->property("exePath").toString();
//    QProcess::startDetached(m_currentApp.exePath);
//}



