#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AppItem.h"
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
    explicit MainWindow(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event) override;
    ~MainWindow() override;

private:
    int l = 0;
    int h = 0;
    AppItem m_currentApp;
    Ui::MainWindow *ui;
    void drawLine(QPainter &painter);
    void setGeometryButton(QToolButton *opener);
    AppItem takeInfo();
private slots:
    void on_openFileButton_clicked();
    //void on_opener_clicked();
};


#endif // MAINWINDOW_H
