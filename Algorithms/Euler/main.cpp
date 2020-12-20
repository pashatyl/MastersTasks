#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <sstream>
#include <set>

using data_t = unsigned long long;

int main() {
    std::ios::sync_with_stdio(false);

    data_t a;
    std::vector<data_t> results;
    std::string line;
    getline(std::cin, line);
    std::istringstream iss(line);
    while (iss >> a)
    {
        results.push_back(a);
    }

    if (results[0] == results[1] || results[0] != results[results.size() - 1]){
        std::cout << "NO";
        return 0;
    }

    std::stack<data_t> stack;
    std::set<data_t> visited;
    stack.push(results[0]);
    visited.insert(results[0]);
    for (data_t i = 1; i < results.size() - 1; ++i) {
        if (!stack.empty() && stack.top() == results[i + 1]){
            if (visited.find(results[i]) == visited.end()){
                visited.insert(results[i]);
            } else {
                break;
            }
            stack.pop();
        } else {
            stack.push(results[i]);
        }
    }

    if (stack.empty()){
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }

    return 0;
}
