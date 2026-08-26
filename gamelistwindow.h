#ifndef GAMELISTWINDOW_H
#define GAMELISTWINDOW_H

#include "AppItem.h"
#include "database.h"
#include "gamecard.h"
#include <QWidget>

class gamelistWindow : public QWidget
{
    Q_OBJECT

public:
    explicit gamelistWindow(QWidget *parent = nullptr);
    void extracted(QVector<AppItem> &savedApps);
private:
    int counterCards = 0;
    int l = 0;
    int h = 0;
    Database db;
    QVector<Gamecard*> cardList;
    AppItem cardInfo;
    void setGeometryButtons();
    AppItem takeInfo();
private slots:
    void on_openFileButton_clicked();
    void onCardRemoveRequested();
};

#endif // GAMELISTWINDOW_H
