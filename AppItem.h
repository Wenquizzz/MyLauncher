#ifndef APPITEM_H
#define APPITEM_H
#include <QString>
using namespace std;

#endif // APPITEM_H

struct AppItem {
    int id = -1;
    QString name;
    QString exePath;
    int buttonId;
};
