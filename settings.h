#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class Settings: public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
signals:
    void changeTheme();
};

#endif // SETTINGS_H
