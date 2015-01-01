#pragma once

#include <string>
#include <vector>

void MatchNaively(const std::string& needle, const std::string& haystack, std::vector<size_t>& answer);
void MatchWithKMP(const std::string& needle, const std::string& haystack, std::vector<size_t>& answer);