#ifndef STEAMSELECTORWINDOW_H
#define STEAMSELECTORWINDOW_H

#include <QDialog>
#include <QScrollArea>
#include "AppItem.h"

class SteamSelectorWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SteamSelectorWindow(const QVector<AppItem> &games, QWidget *parent = nullptr);
    QVector<AppItem> getSelectedGames() const;

private:
    QScrollArea *m_scrollArea;
    QVector<AppItem> m_allGames;
    QVector<AppItem> m_selectedGames;
};

#endif // STEAMSELECTORWINDOW_H