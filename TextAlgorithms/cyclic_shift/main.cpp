#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <complex>
#include <cmath>
using namespace std;

#define MY_M_PI 3.14159265358979323846

void fourier(vector<complex<double>> &a, bool inv) {
    int n = a.size();
    if (n == 1) {
        return;
    }

    int m = n / 2;

    vector<complex<double>> aa(m);
    vector<complex<double>> ab(m);

    for (int i = 0, j = 0; i < n; i += 2, j++) {
        aa[j] = a[i];
        ab[j] = a[i + 1];
    }

    fourier(aa, inv);
    fourier(ab, inv);

    int sign;
    if (inv) sign = -1; else sign = 1;

    double angle = 2 * MY_M_PI / n * sign;

    complex<double> w(1);
    complex<double> wn(cos(angle), sin(angle));

    for (int i = 0; i < m; i++) {
        a[i] = aa[i] + w * ab[i];
        a[i + m] = aa[i] - w * ab[i];
        if (inv) {
            a[i] /= 2;
            a[i + m] /= 2;
        }
        w *= wn;
    }
}

void multiply(const vector<int> &a, const vector<int> &b) {
    vector<complex<double>> fa(a.begin(), a.end());
    vector<complex<double>> fb(b.begin(), b.end());

    int n = 1;
    while (n < max(a.size(), b.size()))  {
        n <<= 1;
    }
    n <<= 1;

    fa.resize(n);
    fb.resize(n);

    fourier(fa, false);
    fourier(fb, false);

    for (int i = 0; i < n; i++) {
        fa[i] *= fb[i];
    }

    fourier(fa, true);

    int last_non_empty = 0;
    for (int i = 0; i < n; i++) {
        int val = lround(fa[i].real());
        if (val != 0) {
            last_non_empty = i;
        }

    }
}


int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int len;
    std::string str1;
    std::string str2;
    fin >> len;
    str1.reserve(4 * len + 10);
    fin >> str1 >> str2;

    //возьмём
    //P'(x) = a0 + a1 x + a2 x^2 + ... + an-1 x^n-1,
    //P(x) = P'(x) * (1 + x^n)
    //Q(x) = bn-1 + bn-2 x + bn-3 x^2 + ... + b0 x^n-1

    char letters {'A', 'B', 'C', 'D'};
    for (int i = 0; i < str1.size() - 1; ++i) {
        for (int j = 0; j < sizeof(letters); ++j) {
            std::vector<bool> p_;
            multiply()
        }
    }


    return 0;
}
