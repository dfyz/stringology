#pragma once

#include <algorithm>
#include <string>
#include <vector>

template<typename TIter>
std::vector<ptrdiff_t> CalcBorders(TIter begin, TIter end) {
	size_t m = std::distance(begin, end);
	std::vector<ptrdiff_t> result(m + 1);

	result[0] = -1;
	ptrdiff_t borderSize = -1;

	for (size_t i = 1; i <= m; i++) {
		while (borderSize >= 0 && begin[borderSize] != begin[i - 1]) {
			borderSize = result[borderSize];
		}
		result[i] = ++borderSize;
	}

	return result;
}

template<typename TIter>
std::vector<size_t> CalcFundamentalPreprocessing(TIter begin, TIter end) {
	size_t m = std::distance(begin, end);
	std::vector<size_t> result(m, 0);

	size_t boxLeft = 0;
	size_t boxRight = 0;
	for (size_t i = 1; i < m; i++) {
		size_t matched = (i > boxRight) ? 0 : std::min(result[i - boxLeft], boxRight - i + 1);
		while ((begin + matched < end) && (begin[matched] == begin[i + matched])) {
			++matched;
		}
		if (matched > 0) {
			boxLeft = i;
			boxRight = i + matched - 1
		}
		result[i] = matched;
	}

	return result;
}