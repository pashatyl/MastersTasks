def merge(arr1, arr2):
    res = []
    l1 = list(arr1)
    l2 = list(arr2)
    while l1 and l2:
        res.append(l1.pop(0) if l1[0] <= l2[0] else l2.pop(0))
    return (type(arr1))(res + l1 + l2)


if __name__ == '__main__':
    assert merge([1, 2, 7], [3]) == [1, 2, 3, 7]
    assert merge((3, 15), (7, 8)) == (3, 7, 8, 15)