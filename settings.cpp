#include "settings.h"
#include <QLabel>
#include <QCheckBox>
#include <QProcess>

Settings::Settings(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 958, 750);
    QLabel *text = new QLabel("There are new function soon", this);
    text->setStyleSheet("color: black;");
    text->show();

}
