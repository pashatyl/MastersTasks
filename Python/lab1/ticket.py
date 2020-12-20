def is_lucky(x):
    s = str(x)
    odd = [int(s[i]) for i in range(0, len(s), 2)]
    even = [int(s[i]) for i in range(1, len(s), 2)]
    return sum(odd) == sum(even)


def get_nearest_lucky_ticket(x):
    x1 = x2 = x
    while True:
        if is_lucky(x1):
            return x1
        if is_lucky(x2):
            return x2
        x1 += 1
        x2 -= 1


if __name__ == '__main__':
    assert get_nearest_lucky_ticket(111111) == 111111
    assert get_nearest_lucky_ticket(123321) == 123321
    assert get_nearest_lucky_ticket(123320) == 123321
    assert get_nearest_lucky_ticket(333999) == 334004
    assert get_nearest_lucky_ticket(303000) == 302995
