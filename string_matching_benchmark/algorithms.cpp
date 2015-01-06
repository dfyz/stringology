#include "algorithms.h"
#include "preprocessing.h"

#include <limits>

#define DEFINE_ALGO(name) DECLARE_ALGO(name)

DEFINE_ALGO(Naive) {
	const TPos maxHaystackPos = n - m;
	for (TPos start = 0; start <= maxHaystackPos; start++) {
		TPos matched = 0;
		while (matched < m && haystack[start + matched] == needle[matched]) {
			matched++;
		}
		if (matched == m) {
			answer.push_back(start);
		}
	}
}

DEFINE_ALGO(KnuthMorrisPratt) {
	std::vector<TPos> borders(CalcBorders(needle.begin(), needle.end()));

	TPos haystackPos = 0;
	TPos needlePrefixSize = 0;
	const TPos maxHaystackPos = n - m;
	while (haystackPos <= maxHaystackPos) {
		while (needlePrefixSize < m && (haystack[haystackPos + needlePrefixSize] == needle[needlePrefixSize])) {
			needlePrefixSize++;
		}
		if (needlePrefixSize == m) {
			answer.push_back(haystackPos);
		}
		const TPos shift = needlePrefixSize - borders[needlePrefixSize];
		haystackPos += shift;
		needlePrefixSize -= shift;
		if (needlePrefixSize < 0) {
			needlePrefixSize = 0;
		}
	}
}

static TPos GetMatchingSuffixSize(const std::string& haystack, const std::string& needle, TPos haystackPos, TPos& suffixStart) {
	TPos result = 0;
	TPos i = needle.length() - 1;
	while (i >= 0 && (haystack[haystackPos] == needle[i])) {
		haystackPos--;
		i--;
		result++;
	}
	suffixStart = haystackPos + 1;
	return result;
}

DEFINE_ALGO(BoyerMooreHorspool) {
	std::vector<TPos> shiftsByChar(std::numeric_limits<char>::max(), m);
	for (TPos i = 0; i + 1 < m; i++) {
		shiftsByChar[needle[i]] = m - i - 1;
	}

	TPos haystackPos = m - 1;
	while (haystackPos < n) {
		TPos suffixStart;
		TPos matched = GetMatchingSuffixSize(haystack, needle, haystackPos, suffixStart);
		if (matched == m) {
			answer.push_back(suffixStart);
		}
		haystackPos += shiftsByChar[haystack[haystackPos]];
	}
}

DEFINE_ALGO(BoyerMoore) {
	std::vector<TPos> rightmostOccurence(std::numeric_limits<char>::max(), -1);
	for (TPos i = 0; i < m; i++) {
		rightmostOccurence[needle[i]] = i;
	}

	std::vector<TPos> prevSuffix;
	std::vector<TPos> suffixBorders;
	CalcBoyerMooreShiftTables(needle.begin(), needle.end(), prevSuffix, suffixBorders);

	TPos haystackPos = m - 1;
	while (haystackPos < n) {
		TPos suffixStart;
		TPos matched = GetMatchingSuffixSize(haystack, needle, haystackPos, suffixStart);
		if (matched == m) {
			answer.push_back(suffixStart);
			haystackPos += (m > 1) ? (m - suffixBorders[1]) : 1;
		} else {
			TPos mismatchPosInNeedle = m - matched - 1;
			TPos badCharacterShift = std::max<TPos>(1, mismatchPosInNeedle - rightmostOccurence[haystack[suffixStart - 1]]);

			TPos goodSuffixShift = 0;
			if (matched > 0) {
				TPos lastMatchPosInNeedle = mismatchPosInNeedle + 1;
				TPos prevSuffixPos = prevSuffix[lastMatchPosInNeedle];
				TPos suffixBorderLength = suffixBorders[lastMatchPosInNeedle];
				goodSuffixShift = (prevSuffixPos != -1) ? (m - 1 - prevSuffixPos) : (m - suffixBorderLength);
			}

			haystackPos += std::max(badCharacterShift, goodSuffixShift);
		}
	}
}

#undef DEFINE_ALGO
