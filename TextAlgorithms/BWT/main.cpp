#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using data_t = unsigned long;
using vector_t = std::vector<data_t>;

vector_t prepare_inv_index(std::string const &bwt_string) {
    vector_t forward(bwt_string.length());
    vector_t inverted(bwt_string.length());

    std::iota(forward.begin(), forward.end(), 0);

    stable_sort(forward.begin(),
                forward.end(),
                [&](int i, int j) { return bwt_string[i] < bwt_string[j]; });

    for (int i = 0; i < bwt_string.length(); i++) {
        inverted[forward[i]] = i;
    }

    return inverted;
}

int main() {
    std::string bwt_string, pattern;
    std::cin >> bwt_string >> pattern;

    auto inv_index = prepare_inv_index(bwt_string);

    data_t top = 0, bottom = bwt_string.length() - 1;
    for (int i = pattern.length() - 1; i >= 0; i--) {
        auto current = pattern[i];
        data_t min = bwt_string.length() - 1;
        data_t max = 0ul;
        auto found = false;

        for (auto j = top; j <= bottom; j++) {
            if (bwt_string[j] == current) {
                found = true;
                min = std::min(min, inv_index[j]);
                max = std::max(max, inv_index[j]);
            }
        }

        if (!found) {
            std::cout << 0;
            return 0;
        }

        top = min;
        bottom = max;
    }

    std::cout << bottom - top + 1;
    return 0;
}