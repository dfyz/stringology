#include "algorithms.h"
#include "preprocessing.h"

#include <limits>

#define DEFINE_ALGO(name) DECLARE_ALGO(name)

DEFINE_ALGO(Naive) {
	const size_t maxHaystackPos = n - m;
	for (size_t start = 0; start <= maxHaystackPos; start++) {
		size_t matched = 0;
		while (matched < m && haystack[start + matched] == needle[matched]) {
			matched++;
		}
		if (matched == m) {
			answer.push_back(start);
		}
	}
}

DEFINE_ALGO(KnuthMorrisPratt) {
	std::vector<ptrdiff_t> borders(CalcBorders(needle.begin(), needle.end()));

	ptrdiff_t haystackPos = 0;
	ptrdiff_t needlePrefixSize = 0;
	const ptrdiff_t maxHaystackPos = n - m;
	while (haystackPos <= maxHaystackPos) {
		while (needlePrefixSize < m && (haystack[haystackPos + needlePrefixSize] == needle[needlePrefixSize])) {
			needlePrefixSize++;
		}
		if (needlePrefixSize == m) {
			answer.push_back(haystackPos);
		}
		const ptrdiff_t shift = needlePrefixSize - borders[needlePrefixSize];
		haystackPos += shift;
		needlePrefixSize -= shift;
		if (needlePrefixSize < 0) {
			needlePrefixSize = 0;
		}
	}
}

DEFINE_ALGO(BoyerMooreHorspool) {
	std::vector<size_t> shiftsByChar(std::numeric_limits<char>::max(), m);
	for (size_t i = 0; i + 1 < m; i++) {
		shiftsByChar[needle[i]] = m - i - 1;
	}

	size_t haystackPos = m - 1;
	while (haystackPos < n) {
		size_t i = haystackPos;
		size_t j = m;
		while (j > 0 && (haystack[i] == needle[j - 1])) {
			i--;
			j--;
		}
		if (j == 0) {
			answer.push_back(i + 1);
		}
		haystackPos += shiftsByChar[haystack[haystackPos]];
	}
}

DEFINE_ALGO(BoyerMoore) {

}

#undef DEFINE_ALGO
