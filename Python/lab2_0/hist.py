def distribute(sample, k):
    slice_weight = 1. / k
    minimum = min(sample)
    maximum = max(sample)
    hist = {i: 0 for i in range(k)}

    for i in sample:
        slice = (i - minimum) // ((maximum - minimum) * slice_weight) if i != maximum else k - 1
        hist[slice] += 1

    return list(hist.values())


if __name__ == '__main__':
    assert distribute([1.25, 1, 2, 1.75], 2) == [2, 2]
