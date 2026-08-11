#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamecard.h"
#include "styles.h"
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

    db.openBase("launcher.db");
    QVector<AppItem> savedApps = db.takeListOfBase();
    for (const AppItem &app : savedApps) {
        QFileIconProvider provider;
        QIcon icon = provider.icon(QFileInfo(app.exePath));
        Gamecard *card = new Gamecard(app.id, app.name, app.exePath, icon, this);
        connect(card, &Gamecard::removeRequested, this, &MainWindow::onCardRemoveRequested);
        cardList.push_back(card);
        card->show();
    }

    setGeometryButtons();

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
    counterCards++;
    data.name = QFileInfo(filePath).baseName();
    data.exePath = filePath;
    data.id = counterCards;
    qDebug() << data.name;
    qDebug() << filePath;
    qDebug() << data.id;
    return data;
}

void MainWindow::setGeometryButtons(){
    int size = cardList.size();
    for(int i = 0; i < size; i++) {
        l = i%5;
        h = i/5;
        int left_indent = 264+(l*182);
        int top_indent = 32+(h*242);
        cardList[i] -> setGeometry(left_indent, top_indent, 150, 210);
    }
}

void MainWindow::on_openFileButton_clicked() {
    cardInfo = takeInfo();

    if (cardInfo.exePath.isEmpty()) {
        return;
    }

    QFileIconProvider provider;
    QIcon icon = provider.icon(QFileInfo(cardInfo.exePath));
    Gamecard *card = new Gamecard(cardInfo.id, cardInfo.name, cardInfo.exePath, icon, this);
    db.addProgramToBase(cardInfo);
    connect(card, &Gamecard::removeRequested, this, &MainWindow::onCardRemoveRequested);
    cardList.push_back(card);
    qDebug() << cardList;
    card->setProperty("exePath", cardInfo.exePath);
    setGeometryButtons();
    card-> show();
}

void MainWindow::onCardRemoveRequested() {
    Gamecard *card = qobject_cast<Gamecard*>(sender());

    if (card) {
        int id = card->getId();
        db.deleteProgram(id);
        cardList.removeOne(card);
        card->deleteLater();
        setGeometryButtons();
    }
}

















