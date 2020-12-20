#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include <vector>

std::vector<QPoint> generatePoints(uint N, int width, int height) {
    std::vector<QPoint> points;
    for (uint i = 0; i < N; i++) {
        points.emplace_back(rand() % width, rand() % height);
    }
    return points;
}

void MainWindow::regeneratePoints(uint N) {
    m_pointsGenerated = false;
    m_points.clear();
    m_points = generatePoints(N, m_ui->widget->width(), m_ui->widget->height());

    if (m_kdtree != nullptr) {
        delete m_kdtree;
    }
    m_kdtree = new KDNode(m_points);

    m_allPointsDrown = false;
    m_isRectangleCreated = false;

    QPoint tmp(0, 0);
    m_rectangle.setTopLeft(tmp);
    m_rectangle.setBottomRight(tmp);
    m_tempRectangle = m_rectangle;

    m_pixmap.fill(Qt::white);

    m_pointsGenerated = true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    m_pixmap = QPixmap(m_ui->widget->width(), m_ui->widget->height());

    m_pixmap.fill(Qt::white);
}

void MainWindow::on_pushButton_clicked() {
    const QString text = m_ui->lineEdit->text();

    bool ok = true;
    m_count = text.toInt(&ok);
    if (!ok) return;

    this->regeneratePoints(m_count);
    update();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (!m_isRectangleCreated) {
        m_rectangle.setTopLeft(event->pos());
        m_rectangle.setBottomRight(event->pos());
    } else {
        m_startMouseMovePoint = event->pos();
        m_tempRectangle = m_rectangle;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if(event->type() == QEvent::MouseMove) {
        if (!m_isRectangleCreated) {
            m_rectangle.setBottomRight(event->pos());
        } else {
            QPoint diff = event->pos() - m_startMouseMovePoint;

            m_rectangle.setBottomRight(m_tempRectangle.bottomRight() + diff);
            m_rectangle.setBottomLeft(m_tempRectangle.bottomLeft() + diff);
            m_rectangle.setTopLeft(m_tempRectangle.topLeft() + diff);
            m_rectangle.setTopRight(m_tempRectangle.topRight() + diff);
        }
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (!m_isRectangleCreated) {
        m_isRectangleCreated = true;
        if (m_rectangle.bottomRight().x() < m_rectangle.topLeft().x()) {
            QPoint temp = m_rectangle.bottomRight();
            m_rectangle.setBottomRight(m_rectangle.topLeft());
            m_rectangle.setTopLeft(temp);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QPainter pixPainter(&m_pixmap);

    QPen pen(Qt::red);
    pen.setWidth(2);

    if (m_pointsGenerated && !m_allPointsDrown) {
        pen.setColor(Qt::black);
        pixPainter.setPen(pen);
        for (QPoint const& point : m_points) {
            pixPainter.drawPoint(point);
        }
        m_allPointsDrown = true;
    }

    painter.drawPixmap(0, 0, m_pixmap);

    if (m_pointsGenerated) {
        pen.setColor(QColorConstants::Svg::green);
        painter.setPen(pen);

        auto const points = m_kdtree->insideRectangle(&m_rectangle);
        this->m_ui->countLabel->setText(QString::number(points.size()));
        for (auto const& point : points)
        {
            painter.drawPoint(point);
        }
        painter.drawRect(m_rectangle);
    }
}

MainWindow::~MainWindow()
{
    delete m_kdtree;
    delete m_ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}
