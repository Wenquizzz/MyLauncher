#include "gamelistwindow.h"
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
#include<QWidget>

gamelistWindow::gamelistWindow(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 958, 750);
    db.openBase("launcher.db");
    QVector<AppItem> savedApps = db.takeListOfBase();
    for (const AppItem &app : savedApps) {
        QFileIconProvider provider;
        QIcon icon = provider.icon(QFileInfo(app.exePath));
        Gamecard *card = new Gamecard(app.id, app.name, app.exePath, icon, this);
        connect(card, &Gamecard::removeRequested, this, &gamelistWindow::onCardRemoveRequested);
        cardList.push_back(card);
        card->show();
    }

    setGeometryButtons();

    QPushButton *openFileButton = new QPushButton("+", this);
    openFileButton ->setGeometry(868, 660, 80, 80);
    openFileButton->setStyleSheet(ADD_CARD_STYLE);

    connect(openFileButton, &QPushButton::clicked, this, &gamelistWindow::on_openFileButton_clicked);;
}

AppItem gamelistWindow::takeInfo() {
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

void gamelistWindow::setGeometryButtons(){
    int size = cardList.size();
    for(int i = 0; i < size; i++) {
        l = i%5;
        h = i/5;
        int left_indent = 32+(l*182);
        int top_indent = 32+(h*242);
        cardList[i] -> setGeometry(left_indent, top_indent, 150, 210);
    }
}

void gamelistWindow::on_openFileButton_clicked() {
    cardInfo = takeInfo();

    if (cardInfo.exePath.isEmpty()) {
        return;
    }

    QFileIconProvider provider;
    QIcon icon = provider.icon(QFileInfo(cardInfo.exePath));
    Gamecard *card = new Gamecard(cardInfo.id, cardInfo.name, cardInfo.exePath, icon, this);
    db.addProgramToBase(cardInfo);
    connect(card, &Gamecard::removeRequested, this, &gamelistWindow::onCardRemoveRequested);
    cardList.push_back(card);
    qDebug() << cardList;
    card->setProperty("exePath", cardInfo.exePath);
    setGeometryButtons();
    card-> show();
}

void gamelistWindow::onCardRemoveRequested() {
    Gamecard *card = qobject_cast<Gamecard*>(sender());

    if (card) {
        int id = card->getId();
        db.deleteProgram(id);
        cardList.removeOne(card);
        card->deleteLater();
        setGeometryButtons();
    }
}
