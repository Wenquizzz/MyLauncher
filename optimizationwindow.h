#ifndef OPTIMIZATIONWINDOW_H
#define OPTIMIZATIONWINDOW_H

#include "database.h"
#include <QWidget>

class optimizationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit optimizationWindow(Database *db, QWidget *parent = nullptr);
private:
    Database *m_db;
private slots:
};

#endif // OPTIMIZATIONWINDOW_H
