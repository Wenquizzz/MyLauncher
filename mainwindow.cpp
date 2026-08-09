#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamecard.h"
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
    openFileButton->setStyleSheet(MENU_CARD_STYLE);

    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::on_openFileButton_clicked);;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    //QPen pen(QColor(0x7C4DFF), 1);
    //painter.setPen(pen);
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

void MainWindow::setGeometryButton(Gamecard *card){
    if (l>4) {
        l=0;
        h++;
    };
    int left_indent = 264+(l*182);
    int top_indent = 32+(h*242);
    card ->setGeometry(left_indent, top_indent, 150, 210);
    l++;
}

void MainWindow::on_openFileButton_clicked() {
    cardInfo = takeInfo();

    if (cardInfo.exePath.isEmpty()) {
        return;
    }

    QFileIconProvider provider;
    QIcon icon = provider.icon(QFileInfo(cardInfo.exePath));

    Gamecard *card = new Gamecard(cardInfo.id, cardInfo.name, cardInfo.exePath, icon, this);
    setGeometryButton(card);
    card->setProperty("exePath", cardInfo.exePath);
    card-> show();
}





















