#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "Predicates.h"

#define RADIUS 300

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(700, 700);
    connect(ui->initButton, &QPushButton::clicked, this, &MainWindow::onInitButtonClicked);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::onSolveButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onInitButtonClicked() {
    std::srand(std::time(0));
    points->clear();
    points->reserve(ui->pointsCount->value());

    while (points->size() < (size_t)ui->pointsCount->value()) {
        double x = 2*((double)rand() / RAND_MAX - 0.5)*RADIUS;
        double y = 2*((double)rand() / RAND_MAX - 0.5)*RADIUS;
        point temp;
        temp.x = x + this->width() / 2;
        temp.y = y + this->height() / 2;
        points->push_back(temp);
    }

    delete hull;
    hull = new graham_hull(*points);
    repaint();
}

bool solve = false;
void MainWindow::onSolveButtonClicked() {
    solve = true;
    repaint();
}

void MainWindow::draw_points(QPainter *painter) {
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    for (auto point: *points) {
        painter->drawPoint(point.x, point.y);
    }
}

void MainWindow::draw_graham_mull(graham_hull &hull, QPainter *painter) {
    auto border = hull.get_border();
    *points = hull.get_points();

    if (border.size() != 0) {
        painter->setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::FlatCap));
        for (size_t i = 0; i < border.size() - 1; ++i) {
            painter->drawLine(
                    (*points)[border[i]].x, (*points)[border[i]].y,
                    (*points)[border[i+1]].x, (*points)[border[i+1]].y);
        }
        painter->drawLine(
                (*points)[border[border.size()-1]].x, (*points)[border[border.size()-1]].y,
                (*points)[border[0]].x, (*points)[border[0]].y);
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (hull != nullptr){
        draw_points(&painter);
    }
    if (solve){
        draw_graham_mull(*hull, &painter);
        solve = false;
    }
}
