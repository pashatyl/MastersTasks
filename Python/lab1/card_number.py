import random

first_numbers = {'visa': 4, 'mastercard': 5}


def get_sum(number):
    sum = int(number[-1])
    for i, e in enumerate(reversed(number[:-1])):
        sum += int(e) if int(i) % 2 else (2 * int(e)) % 9
    return sum


def check_card_number(card_number):
    result = []
    while card_number > 0:
        card_number, rem = divmod(card_number, 10)
        result.append(rem)
    result.reverse()
    return get_sum(result) % 10 == 0


def check_card_number_str(card_number):
    return get_sum(card_number) % 10 == 0


def generate_card_number(type):
    card_number = [first_numbers[type]] + [random.randint(0, 9) for _ in range(1, 16)]
    card_sum = get_sum(''.join(str(e) for e in card_number))
    card_number[-1] = 10 - card_sum % 10
    return ''.join(str(e) for e in card_number)


if __name__ == '__main__':
    assert check_card_number(5082337440657928)
    assert not check_card_number_str('4601496706376197')
    for _ in range(10):
        check_card_number_str(generate_card_number('visa'))
        check_card_number_str(generate_card_number('mastercard'))
