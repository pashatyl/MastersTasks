def transpose(iterable):
    return zip(*iterable)


def scalar_product(first, second):
    try:
        return sum(map(lambda x, y: float(x) * float(y), first, second))
    except ValueError:
        return None
