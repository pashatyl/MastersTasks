def pi_function(s):
    pi = [0 for _ in s]
    for i in range(1, len(s)):
        j = pi[i - 1]
        while j > 0 and s[i] != s[j]:
            j = pi[j - 1]
        pi[i] = j + 1 if s[i] == s[j] else 0
    return pi


if __name__ == '__main__':
    with open('coin.in', 'r') as fin:
        s = fin.read().strip()
        n = len(s)

        p = pi_function(s)
        pow = [1]
        for i in range(1, n + 1):
            pow.append(pow[i - 1] * 2)
        print(pow)
        res = 0
        i = n
        while i > 0:
            res += pow[i]
            i = p[i - 1]

        with open('coin.out', 'w') as fout:
            fout.write(str(res))
