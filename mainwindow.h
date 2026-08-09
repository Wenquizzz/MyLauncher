#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AppItem.h"
#include "gamecard.h"
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
    int counterCards = 0;
    int l = 0;
    int h = 0;
    QVector<Gamecard*> cardList;
    AppItem cardInfo;
    Ui::MainWindow *ui;
    void drawLine(QPainter &painter);
    void setGeometryButtons();
    AppItem takeInfo();
private slots:
    void on_openFileButton_clicked();
    void onCardRemoveRequested();
};


#endif // MAINWINDOW_H
