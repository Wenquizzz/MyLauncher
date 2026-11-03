#include "settings.h"
#include "database.h"
#include "qboxlayout.h"
#include <QLabel>
#include <QCheckBox>
#include <QProcess>

Settings::Settings(Database *db, QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 958, 750);
    QVBoxLayout *settings;
}
