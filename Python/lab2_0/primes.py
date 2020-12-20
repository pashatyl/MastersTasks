def get_primes(n):
    return [x for x in range(2, n + 1) if all(x % y != 0 for y in range(2, x))]


if __name__ == '__main__':
    assert [2, 3, 5, 7, 11] == sorted(get_primes(11))