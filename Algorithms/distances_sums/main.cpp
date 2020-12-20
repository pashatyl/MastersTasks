#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <queue>
#include <numeric>
#include <bitset>

using data_t = unsigned long long;
const data_t chunk_size = sizeof(data_t) * 8;
using bitset_t = std::bitset<chunk_size>;

class row {
    data_t n{};
    data_t chunks_count{};
    std::vector<bitset_t> chunks;
    const data_t not_found = -1;

public:
    [[nodiscard]] data_t size() const {
        return n;
    }

    row &operator=(row const &other) {
        init(other.n);
        chunks = other.chunks;
        return *this;
    };

    explicit row(data_t n) {
        init(n);
    }

    row(data_t n, std::string str) {
        init(n);
        fill(str);
    }

    void init(data_t _n) {
        n = _n;
        chunks_count = ceil(((long double) n) / chunk_size);
        chunks.resize(chunks_count);
        reset();
    }

    void fill(std::string &data) {
        for (data_t i = 0; i < chunks_count; ++i) {
            auto sub = data.substr(i * chunk_size, chunk_size);
            std::reverse(sub.begin(), sub.end());
            chunks[i] = bitset_t(sub);
        }
    }

    data_t get(data_t i) {
        return chunks[i / chunk_size][i % chunk_size];
    }

    void set(data_t i, data_t value) {
        chunks[i / chunk_size][i % chunk_size] = value;
    }

    row operator&(row const &other) const {
        row res(n);
        for (data_t i = 0; i < chunks_count; ++i) {
            res.chunks[i] = chunks[i] & other.chunks[i];
        }
        return res;
    }

    void reset() {
        for (data_t i = 0; i < chunks_count; ++i) {
            chunks[i].reset();
        }
    }

    [[nodiscard]] data_t first() const {
        return next((data_t)-1);
    }

    [[nodiscard]] data_t next(data_t k) const {
        ++k;
        const data_t first_chunk = k / chunk_size;
        for (data_t i = first_chunk; i < chunks_count; ++i) {
            if (chunks[i].any()) {
                data_t j = 0;
                if (i == first_chunk) {
                    j = k % chunk_size;
                }
                for (; j < chunk_size; ++j) {
                    if (chunks[i][j] == 1) {
                        return i * chunk_size + j;
                    }
                }
            }
        }
        return not_found;
    }

    [[nodiscard]] data_t end() const {
        return not_found;
    }

    void cock() {
        for (data_t i = 0; i < chunks_count; ++i) {
            chunks[i].reset();
            chunks[i].flip();
        }
    }
};

data_t bfs(std::vector<row> &data, data_t i) {
    std::queue<data_t> que;
    data_t n = data.size();
    row not_visited(n);
    not_visited.cock();
    std::vector<data_t> distances(n, n);
    row to_visit(n);

    que.push(i);
    distances[i] = 0;
    not_visited.set(i, false);

    while (!que.empty()) {
        data_t idx = que.front();
        que.pop();
        to_visit = data[idx] & not_visited;
        for (data_t j = to_visit.first(); j != to_visit.end(); j = to_visit.next(j)) {
            que.push(j);
            distances[j] = distances[idx] + 1;
            not_visited.set(j, false);
        }
    }

//    for (auto distance : distances) {
//        std::cout << distance << " ";
//    }

    return std::accumulate(
            distances.begin(), distances.end(),
            (data_t)0, [](data_t accum, data_t x) -> data_t { return accum + x * x; });
}

int main() {
    std::ios::sync_with_stdio(false);

    data_t N;
    std::cin >> N;

    std::vector<row> data(N, row(N));
    std::string str;

    for (data_t j = 0; j < N; ++j) {
        std::cin >> str;
        data[j].fill(str);
    }

    data_t sum = 0;
    for (data_t i = 0; i < N; ++i) {
        sum += bfs(data, i);
//        std::cout << sum << std::endl;
    }

    std::cout << sum << std::endl;
    return 0;
}