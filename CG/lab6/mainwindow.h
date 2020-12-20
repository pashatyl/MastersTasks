#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <random>
#include <algorithm>

#include "intersect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsPixmapItem* background;
    QPixmap backgroundImage;
    void redrawBackground();
    int frame_width;
    int frame_height;
    QGraphicsScene *scene;

    int num_of_lines;
    QVector<QLine> generate_lines();
    void draw_lines(QVector<QLine>, QColor);
    void plot_points(std::vector<Point>);
    QVector<QLine> lines;
    bool lines_drawn = false;
    QVector<QVector<QLine>> find_intersections();
};
#endif // MAINWINDOW_H
