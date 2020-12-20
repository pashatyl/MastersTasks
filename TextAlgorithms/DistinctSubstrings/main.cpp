#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<int> z_function(std::string const &s) {
    int j = 0;
    std::vector<int> z(s.size(), 0);

    for (int i = 1; i < s.size(); ++i) {
        int k = i - j;
        z[i] = z[k];
        if (i == 1 || z[k] >= z[j] - k) {
            z[i] = z[j] - k > 0 ? z[j] - k : 0;
            while (i + z[i] < s.size() && s[i + z[i]] == s[z[i]]){
                ++z[i];
//                std::cout << "y " << i + z[i] << " " << z[i] << ",    ";
            }
            j = i;
//            std::cout << "x " << z[i] << ",    ";
        }
//        std::cout << k << " " << j << " " << i << std::endl;
    }
    return z;
}

int main() {
    std::ifstream fin("input.txt");
    std::string input, s;
    fin >> input;
    size_t res = 0;

    for (char i : input) {
        std::string t = s + i;
        s = t;
        std::reverse(t.begin(), t.end());
        auto z = z_function(t);
        auto max = std::max_element(z.begin(), z.end());
        res += t.size() - *max;
    }

    std::ofstream fout("output.txt");
    fout << res;

    return 0;
}
