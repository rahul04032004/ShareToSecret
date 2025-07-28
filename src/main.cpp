#include "../include/shamir.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> testcases = {
        "input/testcase1.json",
        "input/testcase2.json"
    };
    for (const auto& file : testcases) {
        int k;
        std::vector<Share> shares = read_json(file, k);
        if (shares.size() >= (size_t)k && k > 0) {
            long long secret = lagrange_interpolation(shares, k);
            std::cout << secret << std::endl;
        } else {
            std::cout << "Error" << std::endl;
        }
    }
    return 0;
}
