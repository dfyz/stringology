#pragma once

#include <string>
#include <vector>

#include "algorithms.h"
#include "common.h"

#define ALGO(name) Algo_ ## name
#define DECLARE_ALGO(name) void ALGO(name) (const std::string& needle, const std::string& haystack, TPos m, TPos n, std::vector<TPos>& answer)

DECLARE_ALGO(Naive);
DECLARE_ALGO(KnuthMorrisPratt);
DECLARE_ALGO(BoyerMooreHorspool);
DECLARE_ALGO(BoyerMoore);
