import sys
import numpy as np
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.QtGui import QPainter
from PyQt5.QtCore import Qt, QTimer
from math import cos, sin

dAlpha1 = -1
dAlpha2 = 0.01
mul = 100
vector = np.array((200, 200))


def project(point, angle):
    return point[0] + 0.5 * point[2] * cos(angle), point[1] + 0.5 * point[2] * sin(angle)


def rotate3Dx(point, angle):
    return (
        point[0],
        point[1] * cos(angle) - point[2] * sin(angle),
        point[1] * sin(angle) + point[2] * cos(angle)
    )


def rotate3Dy(point, angle):
    return (
        point[0] * cos(angle) - point[2] * sin(angle),
        point[1],
        point[0] * sin(angle) + point[2] * cos(angle)
    )


def rotate3Dz(point, angle):
    return (
        point[0] * cos(angle) - point[1] * sin(angle),
        point[0] * sin(angle) + point[1] * cos(angle),
        point[2]
    )


class Lab3(QWidget):
    def __init__(self, segments, rotation):
        super().__init__()
        self.segments = []
        self.rotation = rotation
        for segment in segments:
            from_point = segment[0] * mul
            to_point = segment[1] * mul
            self.segments.append((from_point, to_point))
        self.initUI()

    def initUI(self):
        self.setGeometry(10, 10, 500, 500)
        self.setWindowTitle('Cabinet')
        self.show()

    def paintEvent(self, event):
        self.segments = self.rotate(self.segments)
        qp = QPainter()
        qp.begin(self)
        qp.setBrush(Qt.red)
        for seg in self.segments:
            from_point = project(seg[0], dAlpha1) + vector
            to_point = project(seg[1], dAlpha1) + vector
            qp.drawLine(*from_point, *to_point)
        qp.end()

    def rotate(self, segments):
        rotated = []
        for segment in segments:
            from_point = self.rotation(segment[0], dAlpha2)
            to_point = self.rotation(segment[1], dAlpha2)
            rotated.append((from_point, to_point))
        return rotated


if __name__ == '__main__':
    app = QApplication(sys.argv)
    data = [
        ((-1, -1, -1), (1, -1, -1)),
        ((-1, -1, -1), (-1, 1, -1)),
        ((-1, -1, -1), (-1, -1, 1)),

        ((1, -1, -1), (1, 1, -1)),
        ((1, -1, -1), (1, -1, 1)),
        ((-1, 1, -1), (1, 1, -1)),

        ((-1, 1, -1), (-1, 1, 1)),
        ((-1, -1, 1), (1, -1, 1)),
        ((-1, -1, 1), (-1, 1, 1)),

        ((1, 1, -1), (1, 1, 1)),
        ((1, -1, 1), (1, 1, 1)),
        ((-1, 1, 1), (1, 1, 1))
    ]

    ex = Lab3(np.array(data), rotate3Dx)
    timer = QTimer()
    timer.timeout.connect(ex.update)
    timer.start(16)
    sys.exit(app.exec_())
