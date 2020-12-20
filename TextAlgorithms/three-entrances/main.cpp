#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<int> z_function_absolute(const char *s, size_t size) {
    int j = 0;
    std::vector<int> z(size, 0);

    for (int i = 1; i < size; ++i) {
        int k = i - j;
        z[i] = z[k];
        if (i == 1 || z[k] >= z[j] - k) {
            z[i] = z[j] - k > 0 ? z[j] - k : 0;
            while (i + z[i] < size && s[i + z[i]] == s[z[i]]) {
                ++z[i];
            }
            j = i;
        }
    }

    for (int i = 1; i < size; ++i) {
        z[i] = std::min(i, z[i]);
    }

    return z;
}

int main() {
    std::ifstream fin("input.txt");
    std::string input;
    fin >> input;

    if (input.empty()) {
        return 0;
    }

    std::vector<std::vector<int>> zs;
    zs.reserve(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        zs.push_back(z_function_absolute(input.c_str() + i, input.size() - i));
    }

    std::vector<int> maxes(input.size(), 0);
    std::transform(zs.begin(), zs.end() - 1, maxes.begin(), [](auto e) {
        return *std::max_element(e.begin() + 1, e.end());
    });

    int begin = 0, end = 0;
    for (int i = 0; i < input.size() - 1; i++) {
        for (int j = 1; j < input.size() - i; j++) {
            if (zs[i][j] > end) {
                auto max = std::min(maxes[j + i], zs[i][j]);
                if (max > end) {
                    begin = i;
                    end = max;
                }
            }
        }
    }

    std::ofstream fout("output.txt");
    fout << input.substr(begin, end);

    return 0;
}
