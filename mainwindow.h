#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AppItem.h"
#include "gamelistwindow.h"
#include "optimizationwindow.h"
#include "settings.h"
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
    ~MainWindow() override;
private:
    QPushButton *openFileButton = new QPushButton("+", this);
    gamelistWindow *m_gamelist;
    optimizationWindow *m_optimizationWindow;
    Settings *m_settings;
    bool isDark = true;
    Ui::MainWindow *ui;
private slots:
    void on_gamelistButton_clicked();
    void on_optimizationWindowButton_clicked();
    void on_settingsbutton_clicked();
    void changeTheme();
};


#endif // MAINWINDOW_H
