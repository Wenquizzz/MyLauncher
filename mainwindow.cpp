#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>

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
    data.name = QFileInfo(filePath).baseName();
    data.exePath = filePath;
    qDebug() << data.name;
    qDebug() << filePath;
    return data;
}

void MainWindow::setGeometryButton(QPushButton *opener){
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
    QPushButton *opener = new QPushButton(m_currentApp.name, this);
    setGeometryButton(opener);
    opener->setProperty("exePath", m_currentApp.exePath);
    opener-> show();

    connect(opener, &QPushButton::clicked, this, [this, opener]() {
        QString path = opener->property("exePath").toString();
        QProcess::startDetached(path);
    });
}

//void MainWindow::on_opener_clicked() {
//    QString path = opener->property("exePath").toString();
//    QProcess::startDetached(m_currentApp.exePath);
//}



