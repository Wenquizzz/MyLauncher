#ifndef STEAMSCANNER_H
#define STEAMSCANNER_H

#include <QVector>
#include <QString>
#include "AppItem.h"

class SteamScanner {
public:
    static QVector<AppItem> scan();
};

#endif // STEAMSCANNER_H