#include "algorithms.h"

void MatchNaively(const std::string& needle, const std::string& haystack, std::vector<size_t>& answer) {
	size_t n = haystack.length();
	size_t m = needle.length();

	for (size_t start = 0; start + m <= n; start++) {
		size_t matched = 0;
		while (matched < m && haystack[start + matched] == needle[matched]) {
			matched++;
		}
		if (matched == m) {
			answer.push_back(start);
		}
	}
}

void MatchWithKMP(const std::string& needle, const std::string& haystack, std::vector<size_t>& answer) {
	const size_t n = haystack.length();
	const size_t m = needle.length();

	std::vector<ssize_t> borders(m + 1);
	borders[0] = -1;

	ssize_t borderSize = -1;
	for (size_t i = 1; i <= m; i++) {
		while (borderSize >= 0 && needle[borderSize] != needle[i - 1]) {
			borderSize = borders[borderSize];
		}
		borders[i] = ++borderSize;
	}

	ssize_t haystackPos = 0;
	ssize_t needlePrefixSize = 0;
	while (haystackPos + m <= n) {
		while (needle[needlePrefixSize] == haystack[haystackPos + needlePrefixSize]) {
			needlePrefixSize++;
		}
		if (needlePrefixSize == m) {
			answer.push_back(haystackPos);
		}
		ssize_t shift = needlePrefixSize - borders[needlePrefixSize];
		haystackPos += shift;
		needlePrefixSize -= shift;
		if (needlePrefixSize < 0) {
			needlePrefixSize = 0;
		}
	}
}