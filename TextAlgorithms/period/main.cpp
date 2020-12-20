#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

using pi_t = std::vector<int>;

pi_t pi_function(std::string const &str) {
    pi_t pi(str.size());
    pi[0] = 0;
    for (int i = 1; i < str.size(); ++i) {
        int j = pi[i - 1];
        while (j > 0 && str[i] != str[j]) {
            j = pi[j - 1];
        }
        pi[i] = str[i] == str[j] ? j + 1 : 0;
    }
    return pi;
}

int main() {
    std::ifstream fin("input.txt");

    int len;
    std::string str;
    fin >> len >> str;

    auto pi = pi_function(str);
//    std::copy(pi.begin(), pi.end(), std::ostream_iterator<int>(std::cout, " "));
//    std::cout << std::endl;

    std::ofstream fout("output.txt");

    for (int i = 2; i <= str.size(); ++i) {
        int k = i - pi[i - 1];
        if (i % k == 0 && i != k) {
            fout << i << " " << i / k << "\n";
        }
    }

    return 0;
}
