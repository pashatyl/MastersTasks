#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();

    frame_width = ui->graphicsView->width();
    frame_height = ui->graphicsView->height();

    QPixmap pixM = QPixmap(frame_width, frame_height);
    pixM.fill(Qt::white);
    background = scene->addPixmap(pixM);
    backgroundImage = pixM;
    ui->graphicsView->setScene(scene);
}


void MainWindow::redrawBackground(){
    background->setPixmap(backgroundImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw_lines(QVector<QLine> lines, QColor color=Qt::black)
{
    auto img = background->pixmap().toImage();
    QPainter nPainter(&img);
    nPainter.setPen(color);
    nPainter.drawLines(lines);
    background->setPixmap(QPixmap::fromImage(img));
    nPainter.end();
}

void MainWindow::plot_points(std::vector<Point> points){
    auto img = background->pixmap().toImage();
    QPainter qPainter(&img);
    qPainter.setPen(Qt::red);
    for (auto point: points){
        qPainter.drawEllipse(QPointF(point.x, point.y), 2, 2);
    }
    background->setPixmap(QPixmap::fromImage(img));
    qPainter.end();
}

QVector<QLine> MainWindow::generate_lines()
{
    QVector<QLine> res(num_of_lines);

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni_x(1, frame_width-1); // guaranteed unbiased
    std::uniform_int_distribution<int> uni_y(1, frame_height-1); // guaranteed unbiased

    std::uniform_int_distribution<int> uni_length(10, 30); // guaranteed unbiased
    std::uniform_int_distribution<int> uni_dir(0, 10); // guaranteed unbiased

    for (int i = 0; i < num_of_lines; i++){
        auto x_1 = uni_x(rng);
        auto y_1 = uni_y(rng);
        int x_2, y_2;
        if (uni_dir(rng) > 5){
            x_2 = x_1;
            y_2 = y_1 + uni_length(rng);
        } else {
            y_2 = y_1;
            x_2 = x_1 + uni_length(rng);
        }
        res[i] = QLine(QPoint(x_1, y_1),
                       QPoint(x_2, y_2));
    }
    return res;
}


void MainWindow::on_pushButton_clicked()
{
    lines_drawn = false;
    redrawBackground();
    num_of_lines = ui->lineEdit->text().toInt();
    lines = generate_lines();
    draw_lines(lines);
    lines_drawn = true;
}


QVector<QVector<QLine>> MainWindow::find_intersections(){
    std::vector<Line> converted_lines;
    converted_lines.reserve(lines.size());
    for (QLine line: lines){
        converted_lines.push_back(Line(Point(line.x1(), line.y1()),
                                       Point(line.x2(), line.y2())));
    }
    std::vector<std::pair<int, int>> intersected_lines_inds_pairs;
    intersected_lines_inds_pairs = get_lines_intersections(converted_lines);
    QVector<QVector<QLine>> result;
    result.reserve(intersected_lines_inds_pairs.size());
    for (std::pair<int, int> i : intersected_lines_inds_pairs){
        result.push_back(QVector<QLine>{lines[i.first], lines[i.second]});
    }
    return result;
}

void MainWindow::on_pushButton_2_clicked()
{
    QVector<QVector<QLine>> intersected_lines_pairs = find_intersections();
    for (auto pair : intersected_lines_pairs){
         draw_lines(pair, Qt::red);
    }
}

