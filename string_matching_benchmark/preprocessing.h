#pragma once

#include "common.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

template<typename TIter>
std::vector<TPos> CalcBorders(TIter begin, TIter end) {
	TPos m = std::distance(begin, end);
	std::vector<TPos> result(m + 1);

	result[0] = -1;
	TPos borderSize = -1;

	for (TPos i = 1; i <= m; i++) {
		while (borderSize >= 0 && begin[borderSize] != begin[i - 1]) {
			borderSize = result[borderSize];
		}
		result[i] = ++borderSize;
	}

	return result;
}

template<typename TIter>
std::vector<TPos> CalcFundamentalPreprocessing(TIter begin, TIter end) {
	TPos m = std::distance(begin, end);
	std::vector<TPos> result(m, 0);

	TPos boxLeft = 0;
	TPos boxRight = 0;
	for (TPos i = 1; i < m; i++) {
		TPos matched = (i > boxRight) ? 0 : std::min(result[i - boxLeft], boxRight - i + 1);
		while ((begin + matched < end) && (begin[matched] == begin[i + matched])) {
			++matched;
		}
		if (matched > 0) {
			TPos newBoxRight = i + matched - 1;
			if (newBoxRight > boxRight) {
				boxLeft = i;
				boxRight = i + matched - 1;
			}
		}
		result[i] = matched;
	}

	return result;
}

template<typename TIter>
void CalcBoyerMooreShiftTables(TIter begin, TIter end, std::vector<TPos>& prevSuffix, std::vector<TPos>& suffixBorders) {
	TPos m = std::distance(begin, end);
	std::vector<TPos> reverseFp(CalcFundamentalPreprocessing(std::reverse_iterator<TIter>(end), std::reverse_iterator<TIter>(begin)));

	prevSuffix.resize(m, -1);
	for (TPos i = m - 1; i >= 0; i--) {
		TPos suffixLen = reverseFp[i];
		if (suffixLen > 0) {
			prevSuffix[m - suffixLen] = m - i - 1;
		}
	}

	suffixBorders.resize(m, 0);
	TPos largestBorder = 0;
	for (TPos i = m - 1; i >= 0; i--) {
		TPos potentialBorderSize = m - i;
		if (reverseFp[i] == potentialBorderSize) {
			largestBorder = potentialBorderSize;
		}
		suffixBorders[i] = largestBorder;
	}
}