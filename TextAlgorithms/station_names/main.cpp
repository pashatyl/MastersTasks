#include <iostream>
#include <array>
#include <queue>
#include <fstream>

class trie {
    static const size_t alphabet_size = 'z' - 'a' + 1;

    static int id(char c) {
        return c - 'a';
    }

public:
    trie() {
        root.suffix_link = &root;
        root.parent = &root;
    }

    struct node {
        std::array<node *, alphabet_size> next{};
        std::array<node *, alphabet_size> go{};
        bool is_terminal = false;
        node *parent = nullptr;
        char pch;
        node *suffix_link = nullptr;

        // Task-specific info
        int max_len = 0;
    };

    void extend(std::string const &str) {
        auto curr = &root;
        for (auto c : str) {
            if (curr->next[id(c)] == nullptr) {
                curr->next[id(c)] = new node;
                curr->next[id(c)]->parent = curr;
                curr->next[id(c)]->pch = c;
            }
            curr = curr->next[id(c)];
        }
        curr->is_terminal = true;
    }

    node *get_link(node *v) {
        if (v->suffix_link == nullptr)
            v->suffix_link = (v == &root || v->parent == &root)
                             ? &root
                             : go(get_link(v->parent), v->pch);
        return v->suffix_link;
    }

    node *go(node *v, char ch) {
        auto c = id(ch);
        if (v->go[c] == nullptr) {
            if (v->next[c] != nullptr) {
                v->go[c] = v->next[c];
            } else {
                v->go[c] = v == &root ? &root : go(get_link(v), ch);
            }
        }
        return v->go[c];
    }

    node root;
};

int main() {
    std::ifstream fin("input.txt");

    int n;
    std::string station;
    trie tr;

    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> station;
        tr.extend(station);
    }
    fin.close();

    std::queue<trie::node *> queue;
    queue.push(&tr.root);
    int res = 1;

    while (!queue.empty()) {
        auto curr = queue.front();
        queue.pop();

        // calculate suffix links here not to went through tree twice
        curr->suffix_link = tr.get_link(curr);

        for (auto node : curr->next) {
            if (node != nullptr) {
                queue.push(node);
            }
        }

        // calculate number of terminal vertices on path to current
        curr->max_len =
                (curr->is_terminal ? 1 : 0) +
                std::max(curr->parent->max_len,
                         curr->suffix_link->max_len);

        res = std::max(res, curr->max_len);
    }

    std::ofstream fout("output.txt");
    fout << res;
    return 0;
}
