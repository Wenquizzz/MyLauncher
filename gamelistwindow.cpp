#include "gamelistwindow.h"
#include "gamecard.h"
#include "steamselectorwindow.h"
#include "steamscanner.h"
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
#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QMessageBox>

gamelistWindow::gamelistWindow(Database *db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setGeometry(0, 0, 958, 750);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setGeometry(0, 0, 958, 750);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    m_container = new QWidget();
    m_container->setStyleSheet("background: transparent;");

    m_gridLayout = new QGridLayout(m_container);
    m_gridLayout->setSpacing(32);
    m_gridLayout->setContentsMargins(32, 32, 32, 32);
    m_gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_scrollArea->setWidget(m_container);

    QVector<AppItem> savedApps = m_db->takeListOfBase();
    for (const AppItem &app : savedApps) {
        QFileIconProvider provider;
        QIcon icon = provider.icon(QFileInfo(app.exePath));
        Gamecard *card = new Gamecard(app.id, app.name, app.exePath, icon, m_container);
        connect(card, &Gamecard::removeRequested, this, &gamelistWindow::onCardRemoveRequested);
        cardList.push_back(card);
        card->show();
    }

    setGeometryButtons();
    QPushButton *openFileButton = new QPushButton("+", this);
    openFileButton->setGeometry(868, 660, 80, 80);
    openFileButton->setStyleSheet(ADD_CARD_STYLE);
    openFileButton->show();
    openFileButton->raise();
    connect(openFileButton, &QPushButton::clicked, this, &gamelistWindow::on_openFileButton_clicked);
    QPushButton *steamButton = new QPushButton("S", this);
    steamButton->setGeometry(748, 660, 80, 80);
    steamButton->setStyleSheet(ADD_CARD_STYLE);
    steamButton->show();
    connect(steamButton, &QPushButton::clicked, this, &gamelistWindow::on_steamButton_clicked);

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
    return data;
}

void gamelistWindow::setGeometryButtons(){
    while (m_gridLayout->count() > 0) {
        m_gridLayout->takeAt(0);
    }

    for (int i = 0; i < cardList.size(); ++i) {
        int row = i / 5;
        int col = i % 5;
        m_gridLayout->addWidget(cardList[i], row, col);
    }
}

void gamelistWindow::on_openFileButton_clicked() {
    cardInfo = takeInfo();

    if (cardInfo.exePath.isEmpty()) {
        return;
    }

    QFileIconProvider provider;
    QIcon icon = provider.icon(QFileInfo(cardInfo.exePath));
    Gamecard *card = new Gamecard(cardInfo.id, cardInfo.name, cardInfo.exePath, icon, m_container);
    m_db->addProgramToBase(cardInfo);
    connect(card, &Gamecard::removeRequested, this, &gamelistWindow::onCardRemoveRequested);
    cardList.push_back(card);
    qDebug() << cardList;
    card->setProperty("exePath", cardInfo.exePath);
    setGeometryButtons();
    card->show();
}

void gamelistWindow::on_steamButton_clicked() {
    QVector<AppItem> foundGames = SteamScanner::scan();

    if (foundGames.isEmpty()) {
        QMessageBox::information(this, "Scan Result", "No Steam games found.");
        return;
    }

    SteamSelectorWindow selector(foundGames, this);
    if (selector.exec() == QDialog::Accepted) {
        QVector<AppItem> selected = selector.getSelectedGames();

        for (const AppItem &app : selected) {
            int newId = m_db->addProgramToBase(app);

            if (newId != -1) {
                QFileIconProvider provider;
                QIcon icon = provider.icon(QFileInfo(app.exePath));

                // Создаем карточку с правильным ID из базы
                Gamecard *card = new Gamecard(newId, app.name, app.exePath, icon, m_container);
                connect(card, &Gamecard::removeRequested, this, &gamelistWindow::onCardRemoveRequested);
                cardList.push_back(card);
                card->show();
            }
        }
        setGeometryButtons();
    }
}

void gamelistWindow::onCardRemoveRequested() {
    Gamecard *card = qobject_cast<Gamecard*>(sender());

    if (card) {
        int id = card->getId();
        m_db->deleteProgram(id);
        cardList.removeOne(card);
        card->deleteLater();
        setGeometryButtons();
    }
}