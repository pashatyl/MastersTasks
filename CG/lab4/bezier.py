from copy import copy


def calculate_barycentric(start_point, end_point, t):
    return ((end_point[0] - start_point[0]) * t + start_point[0],
            (end_point[1] - start_point[1]) * t + start_point[1])


def calculate_bezier_curve_point(control_points, t):
    line = copy(control_points)
    for _ in range(len(control_points) - 1):
        line = [calculate_barycentric(line[i], line[i + 1], t) for i in range(len(line) - 1)]
    return line[0]


def calculate_bezier_curve(control_points):
    size = len(control_points) * 100
    return [calculate_bezier_curve_point(control_points, i / size) for i in range(size)]
