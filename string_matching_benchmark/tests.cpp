// Requires catch.hpp from http://builds.catch-lib.net/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "algorithms.h"
#include "preprocessing.h"

#include <string>

TEST_CASE("border array") {
	{
		std::string pattern("abacaba");
		std::vector<TPos> borders { -1, 0, 0, 1, 0, 1, 2, 3 };
		REQUIRE(CalcBorders(pattern.begin(), pattern.end()) == borders);
	}

	{
		std::string pattern("abababababb");
		std::vector<TPos> borders { -1, 0, 0, 1, 2 , 3, 4, 5, 6, 7, 8, 0 };
		REQUIRE(CalcBorders(pattern.begin(), pattern.end()) == borders);
	}

	{
		std::string pattern("aaaaaa");
		std::vector<TPos> borders { -1, 0, 1, 2, 3, 4, 5 };
		REQUIRE(CalcBorders(pattern.begin(), pattern.end()) == borders);
	}

	{
		std::string patterns("and the lord");
		std::vector<TPos> borders { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		REQUIRE(CalcBorders(patterns.begin(), patterns.end()) == borders);
	}
}

TEST_CASE("fundamental preprocessing (aka z-function)") {
	{
		std::string pattern("aabcaabxaaz");
		std::vector<TPos> fp { 0, 1, 0, 0, 3, 1, 0, 0, 2, 1, 0 };
		REQUIRE(CalcFundamentalPreprocessing(pattern.begin(), pattern.end()) == fp);
	}

	{
		std::string pattern("aaaabaa");
		std::vector<TPos> fp { 0, 3, 2, 1, 0, 2, 1 };
		REQUIRE(CalcFundamentalPreprocessing(pattern.begin(), pattern.end()) == fp);
	}

	{
		std::string pattern("aaaaa");
		std::vector<TPos> fp { 0, 4, 3, 2, 1 };
		REQUIRE(CalcFundamentalPreprocessing(pattern.begin(), pattern.end()) == fp);
	}

	{
		std::string pattern("aaabaab");
		std::vector<TPos> fp { 0, 2, 1, 0, 2, 1, 0 };
		REQUIRE(CalcFundamentalPreprocessing(pattern.begin(), pattern.end()) == fp);
	}

	{
		std::string pattern("abacaba");
		std::vector<TPos> fp { 0, 0, 1, 0, 3, 0, 1 };
		REQUIRE(CalcFundamentalPreprocessing(pattern.begin(), pattern.end()) == fp);
	}

	{
		std::string pattern("aabaabcaxaabaabcy");
		std::vector<TPos> fp { 0, 1, 0, 3, 1, 0, 0, 1, 0, 7, 1, 0, 3, 1, 0, 0, 0 };
		REQUIRE(CalcFundamentalPreprocessing(pattern.begin(), pattern.end()) == fp);
	}

}

TEST_CASE("Boyer-Moore shift tables for the good suffix rule") {
	{
		std::string pattern("cabdabdab");
		std::vector<TPos> prevSuffix { -1, -1, -1, -1, 5, -1, -1, 2, -1 };
		std::vector<TPos> suffixBorders { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		std::vector<TPos> actualPrevSuffix;
		std::vector<TPos> actualSuffixBorders;
		CalcBoyerMooreShiftTables(pattern.begin(), pattern.end(), actualPrevSuffix, actualSuffixBorders);
		REQUIRE(actualPrevSuffix == prevSuffix);
		REQUIRE(actualSuffixBorders == suffixBorders);
	}

	{
		std::string pattern("abacaba");
		std::vector<TPos> prevSuffix { -1, -1, -1, -1, 2, -1, 4 };
		std::vector<TPos> suffixBorders { 3, 3, 3, 3, 3, 1, 1 };

		std::vector<TPos> actualPrevSuffix;
		std::vector<TPos> actualSuffixBorders;
		CalcBoyerMooreShiftTables(pattern.begin(), pattern.end(), actualPrevSuffix, actualSuffixBorders);
		REQUIRE(actualPrevSuffix == prevSuffix);
		REQUIRE(actualSuffixBorders == suffixBorders);
	}

	{
		std::string pattern("cababababa");
		std::vector<TPos> prevSuffix { -1, -1, -1, 7, -1, 5, -1, 3, -1, 1 };
		std::vector<TPos> suffixBorders { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		std::vector<TPos> actualPrevSuffix;
		std::vector<TPos> actualSuffixBorders;
		CalcBoyerMooreShiftTables(pattern.begin(), pattern.end(), actualPrevSuffix, actualSuffixBorders);
		REQUIRE(actualPrevSuffix == prevSuffix);
		REQUIRE(actualSuffixBorders == suffixBorders);
	}
}

TEST_CASE("Boyer-Moore") {
	{
		std::string needle("example");
		std::string haystack("here is a simple example lol");
		std::vector<TPos> result { 17 };

		std::vector<TPos> actualResult;
		ALGO(BoyerMoore)(needle, haystack, needle.length(), haystack.length(), actualResult);
		REQUIRE(actualResult == result);
	}

	{
		std::string needle("spvsameplg");
		std::string haystack("lsvsclqgpcsvfspvsameplgr");
		std::vector<TPos> result { 13 };

		std::vector<TPos> actualResult;
		ALGO(BoyerMoore)(needle, haystack, needle.length(), haystack.length(), actualResult);
		REQUIRE(actualResult == result);
	}

	{
		std::string needle("gll");
		std::string haystack("nmallvgllv");
		std::vector<TPos> result { 6 };

		std::vector<TPos> actualResult;
		ALGO(BoyerMoore)(needle, haystack, needle.length(), haystack.length(), actualResult);
		REQUIRE(actualResult == result);
	}

	{
		std::string needle("abaababaabaa");
		std::string haystack("abaabbbaababaabaabaababaabaa");
		std::vector<TPos> result { 16 };

		std::vector<TPos> actualResult;
		ALGO(BoyerMoore)(needle, haystack, needle.length(), haystack.length(), actualResult);
		REQUIRE(actualResult == result);
	}
}