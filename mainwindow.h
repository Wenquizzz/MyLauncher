#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AppItem.h"
#include "gamelistwindow.h"
#include "optimizationwindow.h"
#include <QMainWindow>
#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void extracted(QVector<AppItem> &savedApps);
    explicit MainWindow(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event) override;
    ~MainWindow() override;
private:
    gamelistWindow *m_gamelist;
    optimizationWindow *m_optimizationWindow;
    Ui::MainWindow *ui;
    void drawLine(QPainter &painter);
private slots:
    void on_gamelistButton_clicked();
    void on_optimizationWindowButton_clicked();
};


#endif // MAINWINDOW_H
