def compress(data):
    result = {}
    for i in data:
        result.setdefault(i, 0)
        result[i] += 1
    return list(result.items())


if __name__ == '__main__':
    expected_sorted = [(1, 2), (2, 1)]
    actual_sorted = sorted(compress([1, 2, 1]))
    assert expected_sorted == actual_sorted
