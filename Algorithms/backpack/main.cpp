#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <algorithm>

using data_t = unsigned long long;

struct item {
    data_t weight;
    data_t cost;
    data_t index;

    void add(item const &item) {
        weight += item.weight;
        cost += item.cost;
    }

    bool operator<(item const &other) const {
        return weight < other.weight;
    }

    std::string toString(){
        return "W " + std::to_string(weight) + ", C " + std::to_string(cost);
    }
};

void fill(std::vector<item> const &items, std::vector<data_t> &results, data_t capacity) {
    std::vector<item> calculated, raw;
    const auto middle = items.begin() + items.size() / 2;
    std::copy(items.begin(), middle, std::back_inserter(calculated));
    std::copy(middle, items.end(), std::back_inserter(raw));

    const data_t size = pow(2, calculated.size());
    std::vector<item> first(size);

    for (data_t i = 0; i < size; ++i) {
        first[i].index = i;
        for (data_t j = 0; j < calculated.size(); ++j) {
            if (((i >> j) & static_cast<data_t>(1)) == 1) {
                first[i].add(calculated[j]);
            }
        }

        //std::cout <<  std::bitset<2>(i) << std::endl << first[i].toString() << std::endl;
    }

    std::sort(first.begin(), first.end());

    item tmp = first[0];
    std::vector<item> filtered;
    filtered.reserve(calculated.size());
    filtered.push_back(tmp);

    for (data_t i = 1; i < size; i++) {
        if (tmp.weight <= first[i].weight && tmp.cost >= first[i].cost) {
            continue;
        }
        filtered.push_back(first[i]);
        tmp = first[i];
    }

    data_t weight = 0, cost = 0, ans = 0, left_max = 0, right_max = 0;
    for (data_t i = 0; i < pow(2, raw.size()); ++i) {
        weight = 0, cost = 0;

        for (data_t j = 0; j < raw.size(); ++j) {
            if (((i >> j) & static_cast<data_t>(1)) == 1) {
                weight += raw[j].weight;
                cost += raw[j].cost;
            }
        }

        item a{capacity - weight, 0};
        data_t index = std::upper_bound(filtered.begin(), filtered.end(), a) - filtered.begin() - 1;
        if (filtered[index].weight + weight <= capacity && filtered[index].cost + cost > ans) {
            ans = filtered[index].cost + cost;
            left_max = filtered[index].index;
            right_max = i;
            //std::cout << "this" << std::endl;
        }
        //std::cout << std::bitset<3>(i) << std::endl << left_max << " " << right_max << " " << weight << std::endl;
    }

    for (data_t i = 0; i < calculated.size(); ++i) {
        if (((left_max >> i) & static_cast<data_t>(1)) == 1) {
            results.push_back(i + 1);
        }
    }

    for (data_t i = 0; i < raw.size(); ++i) {
        if (((right_max >> i) & static_cast<data_t>(1)) == 1) {
            results.push_back(calculated.size() + i + 1);
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    data_t N, W;
    std::cin >> N >> W;
    std::vector<data_t> results;
    results.reserve(N);
    std::vector<item> items;
    items.reserve(N);
    data_t w, c;
    for (data_t j = 0; j < N; ++j) {
        std::cin >> w >> c;
        items.push_back({w, c});
    }

    fill(items, results, W);
    std::cout << results.size() << std::endl;
    std::copy(results.begin(), results.end(), std::ostream_iterator<data_t>(std::cout, " "));
    return 0;
}