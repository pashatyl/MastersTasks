#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <numeric>

using data_t = unsigned int;
using pair_t = std::pair<data_t, data_t>;
using ranges_t = std::vector<pair_t>;
data_t n;

void swap(ranges_t &ranges, ranges_t::iterator it1, ranges_t::iterator it2) {
    if (it1 == ranges.begin()) {
        return;
    }
    std::reverse(ranges.begin(), it1);
    std::reverse(it1, it2);
    std::reverse(ranges.begin(), it2);
}

ranges_t::iterator split(ranges_t &ranges, data_t a) {
    ranges_t::iterator cit;
    data_t i = 0, len, shift;
    auto it = ranges.begin();
    if ((ranges.end() - 1)->second + 1 == a) {
        return ranges.end();
    }
    while (true) {
        len = it->second - it->first + 1;
        if (i + len < a) {
            i += len;
            ++it;
            continue;
        } else {
            cit = it;
            shift = a - i - 1;
            break;
        }
    }

    if (shift == 0) {
        return cit;
    }

    auto end = cit->second;
    cit->second = cit->first + shift - 1;
    ranges.insert(cit + 1, std::make_pair(cit->first + shift, end));
    return cit + 1;
}

std::vector<data_t> flatten(const ranges_t &ranges, const std::vector<data_t> &data) {
    std::vector<data_t> res;
    res.reserve(n);
    for (auto[f, s] : ranges) {
        if (f == s) {
            res.push_back(data[f - 1]);
            continue;
        }
        for (data_t i = f; i <= s; ++i) {
            res.push_back(data[i - 1]);
        }
    }
    return res;
}

int main() {
    std::ios::sync_with_stdio(false);
    data_t q, a, b;
    std::ifstream fin("archive.in");
    fin >> n >> q;

    std::vector<data_t> data(n);
    std::iota(data.begin(), data.end(), 1);

    ranges_t ranges;
    const auto max = sqrt(n);
    ranges.reserve(max * max * max);
    ranges.emplace_back(1, n);
    for (data_t i = 0; i < q; ++i) {
        fin >> a >> b;
        auto it1 = split(ranges, a);
        auto it2 = split(ranges, b + 1);
        swap(ranges, it1, it2);
        if (ranges.size() > max) {
            data = flatten(ranges, data);
            ranges.clear();
            ranges.emplace_back(1, n);
        }
    }

    std::ofstream fout("archive.out");
    auto res = flatten(ranges, data);
    std::copy(res.begin(), res.end(), std::ostream_iterator<data_t>(fout, " "));
    return 0;
}
