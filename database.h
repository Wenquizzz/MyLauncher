#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QVector>
#include "AppItem.h"

class Database {
public:
    Database();
    ~Database();

    bool openBase(const QString &path);
    void closeBase();

    int addProgramToBase(const AppItem &app);
    QVector<AppItem> takeListOfBase();
    bool deleteProgram(int id);
    bool saveTweak(const QString &name, int state);
    int getTweakState(const QString &name);

private:
    QSqlDatabase m_db;
    bool createTable();
};

#endif // DATABASE_H