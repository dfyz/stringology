#pragma once

#include <string>
#include <vector>

#include "algorithms.h"
#include "debug_print.h"

#define ALGO(name) Algo_ ## name
#define DECLARE_ALGO(name) void ALGO(name) (const std::string& needle, const std::string& haystack, size_t m, size_t n, std::vector<size_t>& answer)

DECLARE_ALGO(Naive);
DECLARE_ALGO(KnuthMorrisPratt);
DECLARE_ALGO(BoyerMooreHorspool);
