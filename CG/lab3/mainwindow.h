#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graham_hull.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    std::vector<point> *points = new std::vector<point>;
    graham_hull *hull = nullptr;
    void paintEvent(QPaintEvent *event);
    void onInitButtonClicked();
    void onSolveButtonClicked();
    void draw_points(QPainter *painter);
    void draw_graham_mull(graham_hull &hull, QPainter *painter);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
