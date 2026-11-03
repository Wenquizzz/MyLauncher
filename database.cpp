#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database() {}

bool Database::openBase(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        return false;
    }
    return createTable();
}

bool Database::createTable() {
    QSqlQuery query(m_db);

    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS apps ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT,"
        "exe_path TEXT"
        ")"
        );

    success &= query.exec(
        "CREATE TABLE IF NOT EXISTS tweaks ("
        "tweak_name TEXT PRIMARY KEY, "
        "is_enabled INTEGER"
        ")"
        );

    return success;
}

int Database::addProgramToBase(const AppItem &app) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO apps (name, exe_path) VALUES (:name, :path)");
    query.bindValue(":name", app.name);
    query.bindValue(":path", app.exePath);
    if (query.exec()) {
        return query.lastInsertId().toInt();
    }
    return -1;
}

QVector<AppItem> Database::takeListOfBase(){
    QVector<AppItem> result;
    QSqlQuery query(m_db);
    query.exec("SELECT id, name, exe_path FROM apps");
    while (query.next()) {
        AppItem app;
        app.id = query.value(0).toInt();
        app.name = query.value(1).toString();
        app.exePath = query.value(2).toString();
        result.append(app);
    }
    return result;
}

bool Database::deleteProgram(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM apps WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Database::saveTweak(const QString &name, int state) {
    QSqlQuery query(m_db);
    query.prepare("INSERT OR REPLACE INTO tweaks (tweak_name, is_enabled) VALUES (:name, :state)");
    query.bindValue(":name", name);
    query.bindValue(":state", state);
    return query.exec();
}

int Database::getTweakState(const QString &name) {
    QSqlQuery query(m_db);
    query.prepare("SELECT is_enabled FROM tweaks WHERE tweak_name = :name");
    query.bindValue(":name", name);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

Database::~Database() {}