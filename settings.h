#ifndef SETTINGS_H
#define SETTINGS_H

#include "database.h"
#include <QWidget>

class Settings: public QWidget
{
    Q_OBJECT
public:
    explicit Settings(Database *db, QWidget *parent = nullptr);
private:
    Database *m_db;
signals:
    void changeTheme();
};

#endif // SETTINGS_H
