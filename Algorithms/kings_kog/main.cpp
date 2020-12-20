#include <iostream>
#include <vector>
#include <iterator>

using data_t = long long;

class queue_tree {
public:
    queue_tree() {
        _size = 1e6 + 2;
        tree.resize(_size * 4);
        _build(1, 0, _size - 1);
    }

    void insert(data_t time, data_t duration) {
        _insert(1, 0, _size - 1, time - 1, duration);
    }

    void remove(data_t time, data_t duration) {
        insert(time, -duration);
    }

    data_t query(data_t time) const {
        return std::max(
                _query(1, 0, _size - 1, time - 1, 0) - time + 1,
                static_cast<data_t>(0)); // If is free now
    }

private:
    struct node;

    static data_t left_son(data_t i) {
        return i * 2;
    }

    static data_t right_son(data_t i) {
        return i * 2 + 1;
    }

    void _build(data_t i, data_t left, data_t right) {
        tree[i].left = left;
        tree[i].right = right;

        if (left == right) {
            tree[i].end_time = left;
            return;
        }

        data_t mid = (left + right) / 2;
        _build(left_son(i), left, mid);
        _build(right_son(i), mid + 1, right);
        tree[i].update(tree[left_son(i)], tree[right_son(i)]);
    }

    void _insert(data_t i, data_t left, data_t right, data_t time, data_t duration) {
        if (left == right) {
            tree[i].set(duration);
            return;
        }

        data_t mid = (left + right) / 2;
        if (time <= mid) {
            _insert(left_son(i), left, mid, time, duration);
        } else {
            _insert(right_son(i), mid + 1, right, time, duration);
        }
        tree[i].update(tree[left_son(i)], tree[right_son(i)]);
    }

    data_t _query(data_t i, data_t left, data_t right, data_t time, data_t remaining) const {
        if (right <= time) {
            return tree[i].get_remaining(remaining);
        }

        auto mid = (left + right) / 2;
        remaining = _query(left_son(i), left, mid, time, remaining);
        if (time > mid) {
            remaining = _query(right_son(i), mid + 1, right, time, remaining);
        }
        return remaining;
    }

    struct node {
        data_t duration = 0;
        data_t end_time;
        data_t left;
        data_t right;

        void update(node const &left_son, node const &right_son) {
            duration = left_son.duration + right_son.duration;
            end_time = std::max(left_son.end_time + right_son.duration, right_son.end_time);
        }

        void set(data_t time) {
            duration += time; // += for remove
            end_time += time;
        }

        data_t get_remaining(data_t remaining) const {
            return std::max(remaining + duration, end_time);
        }
    };

public: std::vector<node> tree;
    data_t _size;
};

int main() {
    std::ios::sync_with_stdio(false);
    data_t q;
    std::cin >> q;
    queue_tree tree;
    char key;
    data_t f, s;
    std::vector<std::pair<data_t, data_t>> knights;
    knights.resize(q);
    std::vector<data_t> results;
    results.reserve(q);
    for (data_t j = 0; j < q; ++j) {
//        std::cout << std::endl;
//        for (auto e : tree.tree){
//            std::cout << e.left << " " << e.right << " " << e.duration << " " << e.end_time << std::endl;
//        }
//        std::cout << std::endl;

        std::cin >> key >> f;

        switch (key) {
            case '+':
                std::cin >> s;
                tree.insert(f, s);
                knights[j] = std::make_pair(f, s);
                break;
            case '-':
                tree.remove(knights[f - 1].first, knights[f - 1].second);
                break;
            case '?':
                results.push_back(tree.query(f));
                break;
        }
    }
    std::copy(results.begin(), results.end(), std::ostream_iterator<data_t>(std::cout, "\n"));
    return 0;
}