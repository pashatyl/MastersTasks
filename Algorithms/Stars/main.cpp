#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

using data_t = unsigned long long;

class section_tree {
    struct node;
    data_t _size;
    std::vector<node> _data;

public:
    void init_tree(data_t i, data_t lower, data_t upper) {
        auto &node = _data[i];
        if (upper > lower) {
            auto mid = (upper + lower) / 2;
            //printf("%u %u %u\n", lower, mid, upper);
            init_tree(left_son(i), lower, mid);
            init_tree(right_son(i), mid + 1, upper);

            auto left = _data[left_son(i)];
            auto right = _data[right_son(i)];
            node.left = left.left;
            node.right = right.right;
        } else {
            auto elem = lower;
            //printf("leaf %u %u elem: %u\n", lower, upper, elem);
            node.left = elem;
            node.right = node.left;
            node.value = 0;
        }
    }

    explicit section_tree(int size) {
        _size = 4 * size;
        _data.resize(_size + 1);
        init_tree(1, 0, size - 1);
    }

    static data_t left_son(data_t i) {
        return i * 2;
    }

    static data_t right_son(data_t i) {
        return i * 2 + 1;
    }

    void set(data_t left, data_t right) {
        _set(1, left, right);
    }

    data_t get(data_t left, data_t right) {
        return _perform(1, left, right,
                        [&](data_t i) -> data_t {
                            return _data[i].value;
                        });
    }

private:
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

    void _set(
            const data_t i,
            const data_t left,
            const data_t right) {
        auto &cur = _data[i];

        // Is actual section
        if (left == cur.left && right == cur.right) {
            ++cur.value;
            return;
        }

        auto &left_node = _data[left_son(i)];
        auto &right_node = _data[right_son(i)];
        const data_t left_max = left_node.right;

        data_t direction = left_max >= right ? left_son(i) : right_son(i);
        _set(direction, left, right);
        _data[i].value = left_node.value + right_node.value;
    }

    struct node {
        data_t left;
        data_t right;
        data_t value = 0;
    };
};

int main() {
    std::ios::sync_with_stdio(false);
    data_t n, x, y, level;
    std::ifstream fin("input.txt");
    fin >> n;

    std::vector<data_t> res(n, 0);
    section_tree tree(500'005);
    for (data_t i = 0; i < n; ++i) {
        fin >> x >> y;
        level = tree.get(0, x);
        ++res[level];
        tree.set(x, x);
    }

    std::ofstream fout("output.txt");
    std::copy(res.begin(), res.end(), std::ostream_iterator<data_t>(fout, "\n"));
    return 0;
}
