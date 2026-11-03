#ifndef GAMELISTWINDOW_H
#define GAMELISTWINDOW_H

#include "AppItem.h"
#include "database.h"
#include "gamecard.h"
#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>

class gamelistWindow : public QWidget
{
    Q_OBJECT

public:
    void extracted(QVector<AppItem> &savedApps);
    explicit gamelistWindow(Database *db, QWidget *parent = nullptr);

private:
    int counterCards = 0;
    int l = 0;
    int h = 0;
    QScrollArea *m_scrollArea;
    QWidget *m_container;
    QGridLayout *m_gridLayout;
    Database *m_db;
    QVector<Gamecard*> cardList;
    AppItem cardInfo;
    void setGeometryButtons();
    AppItem takeInfo();
public slots:
    void on_openFileButton_clicked();
private slots:
    void onCardRemoveRequested();
};

#endif // GAMELISTWINDOW_H
