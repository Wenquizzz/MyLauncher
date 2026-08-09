#ifndef GAMECARD_H
#define GAMECARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QVBoxLayout>

class Gamecard: public QWidget
{

Q_OBJECT

public:
    explicit Gamecard(int id, const QString &name, const QString &exePath, const QIcon &icon, QWidget *parent = nullptr);
private:
    int m_id;
    QString m_exePath;
signals:
    void removeRequested(int id);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // GAMECARD_H
