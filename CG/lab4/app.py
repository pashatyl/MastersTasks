import math
import sys
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.QtGui import QPainter, QPen, QColor
import bezier


class Widget(QWidget):
    def __init__(self):
        super().__init__()
        self.qp = QPainter()
        self.move_control_point_idx = None
        self.control_point_grab_area = 15
        self.control_points = [(100, 100), (100, 300)]
        self.initUI()

    def initUI(self):
        self.setGeometry(0, 0, 750, 750)

    def paintEvent(self, e):
        self.qp.begin(self)
        self.show_lines()
        self.show_points(bezier.calculate_bezier_curve(self.control_points))
        self.qp.end()

    def mousePressEvent(self, e):
        for ind, point in enumerate(self.control_points):
            if math.hypot(point[0] - e.x(), point[1] - e.y()) < self.control_point_grab_area:
                self.move_control_point_idx = ind
                break

    def mouseMoveEvent(self, e):
        if self.move_control_point_idx is not None:
            self.control_points[self.move_control_point_idx] = (e.x(), e.y())
            self.repaint()

    def mouseReleaseEvent(self, e):
        self.move_control_point_idx = None

    def mouseDoubleClickEvent(self, e):
        self.control_points.append((e.x(), e.y()))
        self.repaint()

    def show_points(self, points):
        pen = QPen()
        pen.setWidth(2)
        pen.setColor(QColor(200, 0, 0))
        self.qp.setPen(pen)
        for point in points:
            self.qp.drawPoint(point[0], point[1])

    def show_lines(self):
        for i in range(len(self.control_points) - 1):
            self.qp.drawLine(*self.control_points[i], *self.control_points[i + 1])


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Widget()
    ex.show()
    sys.exit(app.exec_())
