#include <iostream>
#include <stack>
#include <vector>
#include <iterator>
#include <limits>

template<typename T>
class minimum_queue {
    using data_t = T;
    using pair_t = std::pair<data_t, data_t>;
    using deque_t = std::deque<pair_t>;
    deque_t asc;
    deque_t desc;

public:
    void push_front(data_t value) {
        push(asc, value);
    }

    void push_back(data_t value) {
        push(desc, value);
    }

    void pop_front() {
        if (asc.empty()) {
            copy(desc, asc);
        }
        asc.pop_front();
    }

    void pop_back() {
        if (desc.empty()) {
            copy(asc, desc);
        }
        desc.pop_front();
    }

    data_t get_min() {
        data_t min;
        if (asc.empty() && desc.empty()) {
            return -1;
        }
        if (asc.empty() || desc.empty()) {
            min = asc.empty() ? desc.front().second : asc.front().second;
        } else {
            min = std::min(asc.front().second, desc.front().second);
        }

        return min;
    }

private:
    static void push(deque_t &deque, const data_t value) {
        const auto min = min_in(deque, value);
        deque.push_front(std::make_pair(value, min));
    }

    static void copy(deque_t &from, deque_t &to) {
        while (from.size() > to.size()) {
            auto e = from.back();
            to.emplace_back(e.first, min_in(to, from.front().first));
            from.pop_back();
        }
        update_minimums(from);
        update_minimums(to);
    }

    static void update_minimums(deque_t &deque){
        data_t min = std::numeric_limits<T>::max();
        for (int i = deque.size() - 1; i >= 0; --i){
            min = std::min(min, deque[i].first);
            deque[i].second = min;
        }
    }

    static data_t min_in(deque_t const &deque, const data_t value) {
        data_t min = value;
        if (!deque.empty()) {
            min = std::min(deque.front().second, value);
        }
        return min;
    }
};

int main() {
    using data_t = long long;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    char key[3];
    data_t x;
    int q;
    std::cin >> q;
    std::vector<data_t> results;
    results.reserve(q);
    minimum_queue<data_t> que;
    for (int i = 0; i < q; ++i) {
        std::cin >> key;
        if (key[0] == '+') {
            std::cin >> x;
            if (key[1] == 'L') {
                que.push_front(x);
            } else {
                que.push_back(x);
            }
        } else {
            if (key[1] == 'L') {
                que.pop_front();
            } else {
                que.pop_back();
            }
        }
        results.push_back(que.get_min());
    }
    std::copy(results.begin(), results.end(), std::ostream_iterator<data_t>(std::cout, "\n"));
    return 0;
}
