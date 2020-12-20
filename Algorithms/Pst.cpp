#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using data_t = unsigned long long;

class pst {
    struct node {
        node *left;
        node *right;
        data_t value;
    };

    static void _init(node *n, data_t left, data_t right) {
        if (left == right) {
            return;
        }
        data_t mid = (left + right) / 2;
        n->left = new node();
        n->right = new node();
        _init(n->left, left, mid);
        _init(n->right, mid + 1, right);
    }

    static void _set(node *prev, node *curr, data_t left, data_t right, data_t i, data_t value) {
        if (i > right || i < left || left > right) {
            return;
        }

        if (left == right) {
            curr->value = value;
            return;
        }

        data_t mid = (left + right) / 2;
        if (i <= mid) {
            curr->right = prev->right;
            curr->left = new node();
            _set(prev->left, curr->left, left, mid, i, value);
        } else {
            curr->left = prev->left;
            curr->right = new node();
            _set(prev->right, curr->right, mid + 1, right, i, value);
        }
        curr->value = curr->left->value + curr->right->value;
    }

    static data_t _get(node *curr, data_t left_bound, data_t right_bound, data_t left, data_t right) {
        if (left_bound > right || right_bound < left || right_bound < left_bound) {
            return static_cast<data_t>(0);
        }

        if (left == left_bound && right == right_bound) {
            return curr->value;
        }

        data_t mid = (left_bound + right_bound) / 2;
        return _get(curr->left, left_bound, mid, left, std::min(right, mid)) +
               _get(curr->right, mid + 1, right_bound, std::max(mid + 1, left), right);
    }

public:
    explicit pst(data_t size) {
        roots.push_back(new node());
        _size = size;
        _init(roots.back(), 0, size);
    }

    void set(data_t i, data_t value) {
        auto prev = roots.back();
        roots.push_back(new node());
        _set(prev, roots.back(), 0, _size - 1, i, value);
    }

    data_t get(data_t left, data_t right, data_t id) {
        return _get(roots[id], 0, _size - 1, left, right);
    }

private:
    std::vector<node *> roots;
    data_t _size;
};

int main() {
    std::ios::sync_with_stdio(false);

    data_t n, q;
    std::cin >> n >> q;

    std::vector<std::pair<data_t, data_t>> elems;
    elems.reserve(n);

    data_t elem;
    for (data_t i = 0; i < n; ++i) {
        std::cin >> elem;
        elems.emplace_back(elem, i);
    }

    std::sort(elems.begin(), elems.end());

    pst tree(n);

    for (auto &e : elems) {
        tree.set(e.second, static_cast<data_t>(1));
    }

    data_t l, r, x, y;
    std::vector<data_t> results;
    for (data_t j = 0; j < q; ++j) {
        std::cin >> l >> r >> x >> y;

        auto left = std::lower_bound(elems.begin(), elems.end(), std::make_pair(x, static_cast<data_t>(0)));
        auto right = std::lower_bound(elems.begin(), elems.end(), std::make_pair(y + 1, static_cast<data_t>(0)));

        results.push_back(
                tree.get(l - 1, r - 1, std::distance(elems.begin(), right)) -
                tree.get(l - 1, r - 1, std::distance(elems.begin(), left)));
    }

    std::copy(results.begin(), results.end(), std::ostream_iterator<data_t>(std::cout, "\n"));

    return 0;
}