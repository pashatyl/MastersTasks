import numpy as np

SIZE = 28
MAX = 255


# helpers
def prepare(a):
    return np.reshape(a, (SIZE, SIZE))


def vertical_split(a):
    a = prepare(a)
    return a[:14, :], a[14:, :]


def submatrix_weight(a, xs, xe, ys, ye):
    a = prepare(a)
    b = a[xs:xe, ys:ye]
    return np.sum(b)


def sum_feature(a):
    """Sum of intensities of all pixels: digits with more used pixels will have higher value."""
    return np.sum(a)


def horizontal_symmetry_feature(a):
    """Subract right part of image from the left part and sum absolute differencies.
    More symmetrical digits will have lower value."""
    a = vertical_split(a)
    b = a[0] - a[1]
    return np.sum(b)


def vertical_symmetry_feature(a):
    a = np.reshape(a, (SIZE, SIZE))
    b = a[:, :14] - a[:, 14:]
    return np.sum(b)


def central_weight_feature(a):
    return submatrix_weight(a, 10, 18, 10, 18)


def lower_central_weight_feature(a):
    return submatrix_weight(a, 18, 24, 10, 18)


def right_diagonal_weight_feature(a):
    a = prepare(a)
    return np.fliplr(a).diagonal().sum()


def left_third_linear_feature(a):
    a = prepare(a)
    left = a[0:(SIZE // 3), 0:SIZE]
    right = a[(SIZE // 3):SIZE, 0:SIZE]
    return np.sum(right) - np.sum(left)


def right_upper_line_feature(a):
    a = prepare(a)
    line = a[int(SIZE * 3 / 7)][::-1]
    ind = np.where(line > MAX * 3 / 7)
    if ind[0].size == 0:
        ind = [[-28]]
    return ind[0][0] ** 4


def sum_bottom_center(a):
    return submatrix_weight(a, 20, 24, 0, SIZE)


def upper_left_line(a):
    return submatrix_weight(a, 10, 11, 7, 14)


def upper_line_feature(a):
    a = prepare(a)
    return np.sum(a[:7, :])


def get_peaks_count(a):
    return len((np.diff(np.sign(np.diff(a))) < 0).nonzero()[0])


def two_lines_crosses_feature(a):
    a = prepare(a)
    return get_peaks_count(a[3 * SIZE // 8]) + get_peaks_count(a[5 * SIZE // 8])


FEATURES = {
    (0, 1): (central_weight_feature, horizontal_symmetry_feature),  #
    (0, 2): (central_weight_feature, horizontal_symmetry_feature),  #
    (0, 3): (central_weight_feature, horizontal_symmetry_feature),  #
    (0, 4): (vertical_symmetry_feature, central_weight_feature),  #
    (0, 5): (horizontal_symmetry_feature, central_weight_feature),  #
    (0, 6): (central_weight_feature, horizontal_symmetry_feature),  #
    (0, 7): (horizontal_symmetry_feature, central_weight_feature),  #
    (0, 8): (central_weight_feature, horizontal_symmetry_feature),  #
    (0, 9): (central_weight_feature, horizontal_symmetry_feature),  #
    (1, 2): (sum_feature, horizontal_symmetry_feature),  #
    (1, 3): (sum_feature, horizontal_symmetry_feature),  #
    (1, 4): (sum_feature, vertical_symmetry_feature),  #
    (1, 5): (sum_feature, horizontal_symmetry_feature),  #
    (1, 6): (sum_feature, horizontal_symmetry_feature),  #
    (1, 7): (sum_feature, horizontal_symmetry_feature),  #
    (1, 8): (sum_feature, horizontal_symmetry_feature),  #
    (1, 9): (sum_feature, horizontal_symmetry_feature),  #
    (2, 3): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (2, 4): (upper_line_feature, horizontal_symmetry_feature),  #
    (2, 5): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (2, 6): (vertical_symmetry_feature, upper_line_feature),
    (2, 7): (sum_feature, horizontal_symmetry_feature),  #
    (2, 8): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (2, 9): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (3, 4): (upper_line_feature, horizontal_symmetry_feature),  #
    (3, 5): (vertical_symmetry_feature, lambda a: submatrix_weight(a, 10, 18, 0, 14)),  #
    (3, 6): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (3, 7): (lambda a: submatrix_weight(a, 12, 16, 11, 17), sum_bottom_center),  #
    (3, 8): (upper_left_line, sum_bottom_center),  #
    (3, 9): (upper_left_line, sum_bottom_center),  #
    (4, 5): (right_upper_line_feature, horizontal_symmetry_feature),  #
    (4, 6): (vertical_symmetry_feature, lower_central_weight_feature),  #
    (4, 7): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (4, 8): (vertical_symmetry_feature, upper_line_feature),  #
    (4, 9): (upper_line_feature, lower_central_weight_feature),
    (5, 6): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (5, 7): (lower_central_weight_feature, vertical_symmetry_feature),
    (5, 8): (two_lines_crosses_feature, right_upper_line_feature),  #
    (5, 9): (right_upper_line_feature, two_lines_crosses_feature),  #
    (6, 7): (lower_central_weight_feature, horizontal_symmetry_feature),  #
    (6, 8): (sum_feature, horizontal_symmetry_feature),  #
    (6, 9): (vertical_symmetry_feature, horizontal_symmetry_feature),  #
    (7, 8): (lower_central_weight_feature, horizontal_symmetry_feature),  #
    (7, 9): (lambda a: submatrix_weight(a, 7, 16, 7, 16), horizontal_symmetry_feature),  #
    (8, 9): (vertical_symmetry_feature, lower_central_weight_feature),  #
}
