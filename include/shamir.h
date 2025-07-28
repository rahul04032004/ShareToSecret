#ifndef SHAMIR_H
#define SHAMIR_H

#include <string>
#include <vector>
#include <map>

typedef std::pair<long long, long long> Share;

std::vector<Share> read_json(const std::string& filename, int& k);
long long decode_value(const std::string& value, int base);
long long lagrange_interpolation(const std::vector<Share>& shares, int k);

#endif
