#ifndef APPITEM_H
#define APPITEM_H
#include <QIcon>
#include <QString>
using namespace std;

struct AppItem {
    int id = -1;
    QString name;
    QString exePath;
};

#endif // APPITEM_H