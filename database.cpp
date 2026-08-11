#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database() {}

bool Database::openBase(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        qDebug() << "Ошибка открытия БД:" << m_db.lastError().text();
        return false;
    }
    return createTable();
}

bool Database::createTable() {
    QSqlQuery query(m_db);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS apps ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT,"
        "exe_path TEXT"
        ")"
    );
}

bool Database::addProgramToBase(const AppItem &app) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO apps (name, exe_path) VALUES (:name, :path)");
    query.bindValue(":name", app.name);
    query.bindValue(":path", app.exePath);
    return query.exec();
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

Database::~Database() {}