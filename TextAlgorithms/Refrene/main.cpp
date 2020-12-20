#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// Copy-pasted from https://www.geeksforgeeks.org/%C2%AD%C2%ADkasais-algorithm-for-construction-of-lcp-array-from-suffix-array/
struct suffix {
    int index;
    int rank[2];
};

int cmp(struct suffix a, struct suffix b) {
    return (a.rank[0] == b.rank[0]) ? (a.rank[1] < b.rank[1] ? 1 : 0) :
           (a.rank[0] < b.rank[0] ? 1 : 0);
}

vector<int> buildSuffixArray(string const &txt) {
    int n = txt.length();
    struct suffix suffixes[n];

    for (int i = 0; i < n; i++) {
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i] - 'a';
        suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - 'a') : -1;
    }

    sort(suffixes, suffixes + n, cmp);

    int ind[n];
    for (int k = 4; k < 2 * n; k = k * 2) {
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < n; i++) {
            if (suffixes[i].rank[0] == prev_rank &&
                suffixes[i].rank[1] == suffixes[i - 1].rank[1]) {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            } else {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }

        for (int i = 0; i < n; i++) {
            int nextindex = suffixes[i].index + k / 2;
            suffixes[i].rank[1] = (nextindex < n) ?
                                  suffixes[ind[nextindex]].rank[0] : -1;
        }

        sort(suffixes, suffixes + n, cmp);
    }

    vector<int> suffixArr;
    for (int i = 0; i < n; i++)
        suffixArr.push_back(suffixes[i].index);

    return suffixArr;
}

vector<int> kasai(string txt, vector<int> suffixArr) {
    int n = suffixArr.size();
    vector<int> lcp(n, 0);
    vector<int> invSuff(n, 0);

    for (int i = 0; i < n; i++)
        invSuff[suffixArr[i]] = i;

    int k = 0;

    for (int i = 0; i < n; i++) {
        if (invSuff[i] == n - 1) {
            k = 0;
            continue;
        }

        int j = suffixArr[invSuff[i] + 1];

        while (i + k < n && j + k < n && txt[i + k] == txt[j + k])
            k++;

        lcp[invSuff[i]] = k;

        if (k > 0)
            k--;
    }

    return lcp;
}

int getMaxArea(std::vector<int> hist, int &max_val, int &max_len, int &max_i) {
    int n = hist.size();
    stack<int> s;

    int max_area = 0;
    int tp;
    int area_with_top;
    int i = 0;
    while (i < n - 1) {
        if (s.empty() || hist[s.top()] <= hist[i]) {
//            std::cout << i << std::endl;
            s.push(i++);
        } else {
            tp = s.top();
            s.pop();
            auto len = (s.empty() ? i : i - s.top() - 1) + 1;
            auto val = hist[tp];
            area_with_top = val * len;

            if (max_area < area_with_top) {
                max_area = area_with_top;
                max_val = val;
                max_len = len;
                max_i = i - len;
            }
        }
    }

    while (!s.empty()) {
        tp = s.top();
        s.pop();
        auto len = (s.empty() ? i : i - s.top() - 1) + 1;
        auto val = hist[tp];
        area_with_top = val * len;

        if (max_area < area_with_top) {
            max_area = area_with_top;
            max_val = val;
            max_len = len;
            max_i = i - len;
        }
    }

    return max_area;
}

int main() {
    std::string str;
    std::cin >> str;

    auto sa = buildSuffixArray(str);
    auto lcp = kasai(str, sa);

//    for (auto i : sa) {
//        std::cout << str.c_str() + i << std::endl;
//    }
//    std::cout << std::endl;
//
//    for (auto i : lcp) {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl;

    int max_val, max_len, max_i;
    auto area = getMaxArea(lcp, max_val, max_len, max_i);

    if (area < str.length()) {
        std::cout << 1 << " " << str.length() << std::endl;
        std::cout << str;
        return 0;
    }

    std::cout << max_len << " " << max_val << std::endl;
    std::cout << str.substr(max_i, max_val);

    return 0;
}
