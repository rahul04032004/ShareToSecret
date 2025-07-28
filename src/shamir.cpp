#include "../include/shamir.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

long long decode_value(const std::string& value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit = isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
        result = result * base + digit;
    }
    return result;
}

// Helper to trim whitespace and quotes
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\"\n\r");
    size_t end = s.find_last_not_of(" \t\"\n\r");
    if (start == std::string::npos || end == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

std::vector<Share> read_json(const std::string& filename, int& k) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Could not open file: " << filename << std::endl;
        k = 0;
        return {};
    }
    std::string line, json;
    while (std::getline(in, line)) json += line;

    // Try to find 'points' array format
    if (json.find("\"points\"") != std::string::npos) {
        // Find n and k
        size_t n_pos = json.find("\"n\"");
        size_t k_pos = json.find("\"k\"");
        int n = 0;
        k = 0;
        if (n_pos != std::string::npos) {
            size_t colon = json.find(":", n_pos);
            n = std::stoi(trim(json.substr(colon + 1, json.find(",", colon) - colon - 1)));
        }
        if (k_pos != std::string::npos) {
            size_t colon = json.find(":", k_pos);
            k = std::stoi(trim(json.substr(colon + 1, json.find(",", colon) - colon - 1)));
        }
        std::vector<Share> shares;
        size_t points_pos = json.find("\"points\"");
        size_t arr_start = json.find('[', points_pos);
        size_t arr_end = json.find(']', arr_start);
        std::string arr = json.substr(arr_start + 1, arr_end - arr_start - 1);
        std::istringstream arr_stream(arr);
        std::string point;
        while (std::getline(arr_stream, point, '{')) {
            if (point.find("\"x\"") == std::string::npos) continue;
            size_t x_pos = point.find("\"x\"");
            size_t y_pos = point.find("\"y\"");
            size_t base_pos = point.find("\"base\"");
            if (x_pos == std::string::npos || y_pos == std::string::npos || base_pos == std::string::npos) continue;
            size_t x_colon = point.find(":", x_pos);
            size_t y_colon = point.find(":", y_pos);
            size_t base_colon = point.find(":", base_pos);
            std::string x_str = trim(point.substr(x_colon + 1, point.find(",", x_colon) - x_colon - 1));
            std::string y_str = trim(point.substr(y_colon + 1, point.find(",", y_colon) - y_colon - 1));
            std::string base_str = trim(point.substr(base_colon + 1, point.find_first_of(",}", base_colon) - base_colon - 1));
            int x = std::stoi(x_str);
            int base = std::stoi(base_str);
            long long y = decode_value(y_str, base);
            shares.emplace_back(x, y);
        }
        return shares;
    }
    // Try to find 'keys' + numbered format
    else if (json.find("\"keys\"") != std::string::npos) {
        size_t keys_pos = json.find("\"keys\"");
        size_t n_pos = json.find("\"n\"", keys_pos);
        size_t k_pos = json.find("\"k\"", keys_pos);
        int n = 0;
        k = 0;
        if (n_pos != std::string::npos) {
            size_t colon = json.find(":", n_pos);
            n = std::stoi(trim(json.substr(colon + 1, json.find(",", colon) - colon - 1)));
        }
        if (k_pos != std::string::npos) {
            size_t colon = json.find(":", k_pos);
            k = std::stoi(trim(json.substr(colon + 1, json.find_first_of(",}", colon) - colon - 1)));
        }
        std::vector<Share> shares;
        for (int i = 1; i <= n; ++i) {
            std::string key = "\"" + std::to_string(i) + "\"";
            size_t key_pos = json.find(key);
            if (key_pos == std::string::npos) continue;
            size_t base_pos = json.find("\"base\"", key_pos);
            size_t value_pos = json.find("\"value\"", key_pos);
            if (base_pos == std::string::npos || value_pos == std::string::npos) continue;
            size_t base_colon = json.find(":", base_pos);
            size_t value_colon = json.find(":", value_pos);
            std::string base_str = trim(json.substr(base_colon + 1, json.find_first_of(",}", base_colon) - base_colon - 1));
            std::string value_str = trim(json.substr(value_colon + 1, json.find_first_of(",}", value_colon) - value_colon - 1));
            int base = std::stoi(base_str);
            long long y = decode_value(value_str, base);
            shares.emplace_back(i, y);
        }
        return shares;
    }
    // Unknown format
    else {
        std::cerr << "Unknown JSON format in file: " << filename << std::endl;
        k = 0;
        return {};
    }
}

long long lagrange_interpolation(const std::vector<Share>& shares, int k) {
    long long secret = 0;
    for (int i = 0; i < k; ++i) {
        long long xi = shares[i].first;
        long long yi = shares[i].second;
        long long num = 1, den = 1;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                num *= -shares[j].first;
                den *= (xi - shares[j].first);
            }
        }
        secret += yi * num / den;
    }
    return secret;
}
