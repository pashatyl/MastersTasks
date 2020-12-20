#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QMouseEvent>

#include <vector>

#include "node.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

    void regeneratePoints(uint N);
private slots:
    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *m_ui;
    QPixmap m_pixmap;

    std::vector<QPoint> m_points;
    uint m_count = 0;
    KDNode* m_kdtree = nullptr;
    QRect m_rectangle;

    QPoint m_startMouseMovePoint;
    bool m_isRectangleCreated = false;
    bool m_allPointsDrown = false;
    bool m_pointsGenerated = false;

    QRect m_tempRectangle;
};

#endif // MAINWINDOW_H
