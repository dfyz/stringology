// Requires catch.hpp from http://builds.catch-lib.net/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

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