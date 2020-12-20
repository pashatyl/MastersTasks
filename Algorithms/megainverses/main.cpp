#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>

using data_t = unsigned long long;

class section_tree {
    struct node;
    data_t _size;
    data_t _data_size;
    std::vector<node> _data;
    using vector_t = std::vector<data_t>;

public:
    void init_tree(data_t i, data_t lower, data_t upper, const std::vector<data_t> &data) {
        auto &node = _data[i];
        if (upper > lower) {
            auto mid = (upper + lower) / 2;
            init_tree(left_son(i), lower, mid, data);
            init_tree(right_son(i), mid + 1, upper, data);

            auto left = _data[left_son(i)];
            auto right = _data[right_son(i)];
            node.left = left.left;
            node.right = right.right;
            node.data.resize(left.data.size() + right.data.size());
            std::merge(left.data.begin(), left.data.end(), right.data.begin(), right.data.end(), node.data.begin());
        } else {
            auto elem = lower;
            auto val = data[elem - 1];
            node.left = elem;
            node.right = node.left;
            node.data.push_back(val);
        }
    }

    explicit section_tree(
            const std::vector<data_t> &data) {
        _size = 4 * data.size();
        _data_size = data.size();
        _data.resize(_size + 1);
        init_tree(1, 1, _data_size, data);
    }

    data_t get_right_count(data_t i, data_t value) {
        return _perform(1, i + 1, _data_size,
                        [&](data_t i) -> data_t {
                            auto &v = _data[i].data;
                            auto it = std::lower_bound(v.begin(), v.end(), value);
                            return it - v.begin();
                        });
    }

    data_t get_left_count(data_t i, data_t value) {
        return _perform(1, 1, i + 1,
                        [&](data_t i) -> data_t {
                            auto &v = _data[i].data;
                            auto it = std::upper_bound(v.begin(), v.end(), value);
                            return v.end() - it;
                        });
    }

private:
    static data_t left_son(data_t i) {
        return i * 2;
    }

    static data_t right_son(data_t i) {
        return i * 2 + 1;
    }

    template<typename F>
    data_t _perform(
            const data_t i,
            const data_t left,
            const data_t right,
            const F &func) const {
        auto &cur = _data[i];

        // Is actual section
        if (left == cur.left && right == cur.right) {
            return func(i);
        }

        auto &left_node = _data[left_son(i)];
        auto &right_node = _data[right_son(i)];
        const data_t left_max = left_node.right;
        const data_t right_min = right_node.left;

        if (left_max >= right || right_min <= left) {
            data_t direction = left_max >= right ? left_son(i) : right_son(i);
            return _perform(direction, left, right, func);
        }
        return _perform(left_son(i), left, left_max, func) +
               _perform(right_son(i), right_min, right, func);
    }

    struct node {
        data_t left;
        data_t right;
        vector_t data;
    };
};


int main() {
    std::ios::sync_with_stdio(false);
    data_t n;
    std::ifstream fin("/Users/pashatyl/Documents/Study/megainverses/input.txt");
    fin >> n;
    std::vector<data_t> data(n);
    for (data_t i = 0; i < n; ++i) {
        fin >> data[i];
    }
    section_tree tree(data);
    data_t count = 0, a, b;

    for (data_t i = 1; i < n - 1; ++i) {
        a = tree.get_left_count(i, data[i]);
        b = tree.get_right_count(i, data[i]);
        count += a * b;
    }

    std::ofstream fout("/Users/pashatyl/Documents/Study/megainverses/output.txt");
    fout << count;
    return 0;
}
