def calculate_special_sum(n):
    return sum([(i - 1) ** 2 * i for i in range(1, n + 1)])


if __name__ == '__main__':
    assert calculate_special_sum(3) == 14
