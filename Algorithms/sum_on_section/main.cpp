#include <iostream>
#include <vector>
#import <cmath>
#include <iterator>
#include <fstream>

using data_t = unsigned long long;

class section_tree {
    struct node;
    data_t _size;
    data_t _data_size;
    std::vector<node> _data;

public:
    static bool is_odd(data_t val) {
        return val % 2 == 1;
    }

    void init_tree(data_t i, data_t lower, data_t upper, const std::vector<data_t> &data) {
        auto &node = _data[i];
        if (upper > lower) {
            auto mid = (upper + lower) / 2;
            //printf("%u %u %u\n", lower, mid, upper);
            init_tree(left_son(i), lower, mid, data);
            init_tree(right_son(i), mid + 1, upper, data);

            auto left = _data[left_son(i)];
            auto right = _data[right_son(i)];
            node.left = left.left;
            node.right = right.right;
            node.odd_count = left.odd_count + right.odd_count;
            node.odd_sum = left.odd_sum + right.odd_sum;
            node.even_count = left.even_count + right.even_count;
            node.even_sum = left.even_sum + right.even_sum;
        } else {
            auto elem = lower;
            //printf("leaf %u %u elem: %u\n", lower, upper, elem);
            auto val = data[elem - 1];
            node.left = elem;
            node.right = node.left;
            node.set(val);
        }
    }

    explicit section_tree(
            const std::vector<data_t> &data) {
//        _size = pow(2, floor(log(data.size()) / log(2))) + data.size();
        _size = 4 * data.size();
        _data_size = data.size();
        _data.resize(_size + 1);
        init_tree(1, 1, _data_size, data);
    }

    static data_t left_son(data_t i) {
        return i * 2;
    }

    static data_t right_son(data_t i) {
        return i * 2 + 1;
    }

    static data_t father(data_t i) {
        return floor(i / 2);
    }

    data_t get_first_leaf() const {
        return _size - _data_size;
    }

    data_t get_leaf(data_t i) const {
        return get_first_leaf() + i - 1;
    }

    void inc(data_t left, data_t right) {
        _inc(1, left, right);
    }

    void set(data_t elem, data_t value) {
        _set(1, elem, value);
    }

    data_t get_evens_sum(data_t left, data_t right) {
        return _perform(1, left, right, 0,
                        [&](data_t i, data_t inc) -> data_t {
                            return _data[i].get_even_sum(inc);
                        });
    }

    data_t get_odds_sum(data_t left, data_t right) {
        return _perform(1, left, right, 0,
                        [&](data_t i, data_t inc) -> data_t {
                            return _data[i].get_odd_sum(inc);
                        });
    }

private:
    template<typename F>
    data_t _perform(
            const data_t i,
            const data_t left,
            const data_t right,
            const data_t inc,
            const F &func) const {
        auto &cur = _data[i];

        // Is actual section
        if (left == cur.left && right == cur.right) {
            return func(i, inc);
        }

        auto &left_node = _data[left_son(i)];
        auto &right_node = _data[right_son(i)];
        const data_t left_max = left_node.right;
        const data_t right_min = right_node.left;

        if (left_max >= right || right_min <= left) {
            data_t direction = left_max >= right ? left_son(i) : right_son(i);
            return _perform(direction, left, right, inc + cur.inc_count, func);
        }
        return _perform(left_son(i), left, left_max, inc + cur.inc_count, func) +
               _perform(right_son(i), right_min, right, inc + cur.inc_count, func);
    }

    void _inc(
            const data_t i,
            const data_t left,
            const data_t right) {
        auto &cur = _data[i];

        // Is actual section
        if (left == cur.left && right == cur.right) {
            ++_data[i].inc_count;
            if (left < right) {
                _recalc_sums(cur, _data[left_son(i)], _data[right_son(i)]);
            }
            return;
        }

        auto &left_node = _data[left_son(i)];
        auto &right_node = _data[right_son(i)];
        const data_t left_max = left_node.right;
        const data_t right_min = right_node.left;

        left_node.inc_count += cur.inc_count;
        right_node.inc_count += cur.inc_count;
        cur.inc_count = 0;

        if (left_max >= right || right_min <= left) {
            data_t direction = left_max >= right ? left_son(i) : right_son(i);
            _inc(direction, left, right);
            _recalc_sums(cur, left_node, right_node);
            return;
        }

        _inc(left_son(i), left, left_max);
        _inc(right_son(i), right_min, right);

        _recalc_sums(cur, left_node, right_node);
    }

    void _set(
            const data_t i,
            const data_t elem,
            const data_t value) {

        auto &cur = _data[i];

        if (cur.value != -1) {
            cur.set(value);
            return;
        }

        auto &left_node = _data[left_son(i)];
        auto &right_node = _data[right_son(i)];
        const data_t left_max = left_node.right;
        const data_t right_min = right_node.left;

        left_node.inc_count += cur.inc_count;
        right_node.inc_count += cur.inc_count;
        cur.inc_count = 0;

        data_t direction = left_max >= elem ? left_son(i) : right_son(i);
        _set(direction, elem, value);
        _recalc_sums(cur, left_node, right_node);
    }

    static void _recalc_sums(node &cur, node const &left, const node &right) {
        cur.odd_sum = left.get_odd_sum() + right.get_odd_sum();
        cur.odd_count = left.get_odd_count() + right.get_odd_count();
        cur.even_sum = left.get_even_sum() + right.get_even_sum();
        cur.even_count = left.get_even_count() + right.get_even_count();
    }

    struct node {
        data_t left;
        data_t right;
        data_t odd_sum = 0;
        data_t odd_count = 0;
        data_t even_sum = 0;
        data_t even_count = 0;
        data_t inc_count = 0;
        data_t value = -1;

        void set(data_t _value) {
            if (is_odd(_value)) {
                odd_sum = _value;
                even_sum = 0;
                odd_count = 1;
                even_count = 0;
            } else {
                even_sum = _value;
                odd_sum = 0;
                even_count = 1;
                odd_count = 0;
            }
            value = _value;
            inc_count = 0;
        }

        data_t get_even_sum(data_t inc = 0) const {
            if (!is_odd(inc_count + inc)) {
                return even_sum + even_count * (inc_count + inc);
            } else {
                return odd_sum + odd_count * (inc_count + inc);
            }
        }

        data_t get_odd_sum(data_t inc = 0) const {
            if (!is_odd(inc_count + inc)) {
                return odd_sum + odd_count * (inc_count + inc);
            } else {
                return even_sum + even_count * (inc_count + inc);
            }
        }

        data_t get_even_count() const {
            if (!is_odd(inc_count)) {
                return even_count;
            } else {
                return odd_count;
            }
        }

        data_t get_odd_count() const {
            if (!is_odd(inc_count)) {
                return odd_count;
            } else {
                return even_count;
            }
        }
    };
};


int main() {
    std::ios::sync_with_stdio(false);
    data_t n, q;
    std::cin >> n >> q;
    std::vector<data_t> data(n);
    for (data_t i = 0; i < n; ++i) {
        std::cin >> data[i];
    }
    section_tree tree(data);
    data_t key, f, s;
    std::vector<data_t> results;
    results.reserve(q);
    for (data_t j = 0; j < q; ++j) {
        std::cin >> key >> f >> s;
        switch (key) {

            case 1:
                tree.set(f, s);
                break;
            case 2:
                tree.inc(f, s);
                break;
            case 3:
                results.push_back(tree.get_evens_sum(f, s));
                break;
            case 4:
                results.push_back(tree.get_odds_sum(f, s));
                break;
        }
    }
    std::copy(results.begin(), results.end(), std::ostream_iterator<data_t>(std::cout, "\n"));
    return 0;
}
