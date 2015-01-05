#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cctype>
#include <cstring>
#include <ctime>

#include "algorithms.h"

using TMatcher = decltype(&ALGO(Naive));

struct TBenchmarkData {
	std::string AlgoName;
	TMatcher Matcher;
	std::vector<double> Times;

	TBenchmarkData(const std::string& algoName, TMatcher matcher)
		: AlgoName(algoName)
		, Matcher(matcher)
	{}
};

void InplaceToLower(std::string& str) {
	for (auto& ch: str) {
		ch = tolower(ch);
	}
}

std::string ReadStringFromFile(const char* fileName) {
	std::ifstream in(fileName);
	if (!in.good()) {
		throw std::runtime_error("Failed to open " + std::string(fileName));
	}
	std::stringstream inStr;
	inStr << in.rdbuf();
	std::string result(inStr.str());

	InplaceToLower(result);
	return result;
}

double MeasureTime(const std::string& matcherName, TMatcher& matcher, const std::string& needle, const std::string& haystack, std::vector<size_t>& result) {
	clock_t start = clock();
	matcher(needle, haystack, needle.size(), haystack.size(), result);
	clock_t end = clock();
	return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000;
}

template<typename T>
T GetMean(const std::vector<T>& items) {
	return items[items.size() / 2];
}

int main(int argc, char** argv) {
	if (argc != 4) {
		std::cerr << "Usage: TEXT NEEDLE_SIZE ITERATION_COUNT" << std::endl;
		return 1;
	}

	srand(time(NULL));

	std::string haystack(ReadStringFromFile(argv[1]));
	const size_t NEEDLE_SIZE = std::stoll(argv[2]);
	const size_t ITERATION_COUNT = std::stoll(argv[3]);

	TBenchmarkData benchmarkDatas[] = {
		{ "Bruteforce", ALGO(Naive) },
		{ "Knuth-Morris-Pratt", ALGO(KnuthMorrisPratt) },
		{ "Boyer-Moore-Horspool", ALGO(BoyerMooreHorspool) },
	};

	std::vector<size_t> matchCounts;
	std::default_random_engine gen;
	for (size_t iter = 0; iter < ITERATION_COUNT; iter++) {
		std::uniform_int_distribution<size_t> randomPos(0, haystack.length() - NEEDLE_SIZE);
		std::string needle(haystack.substr(randomPos(gen), NEEDLE_SIZE));

		std::vector<size_t> correctResult;
		ALGO(Naive)(needle, haystack, needle.size(), haystack.size(), correctResult);
		matchCounts.push_back(correctResult.size());

		for (auto& bd: benchmarkDatas) {
			std::vector<size_t> result;
			auto elapsed = MeasureTime(bd.AlgoName, bd.Matcher, needle, haystack, result);
			bd.Times.push_back(elapsed);

			if (correctResult != result) {
				std::cerr << bd.AlgoName + " gives incorrect results for needle '" << needle << "'" << std::endl;
				return 1;
			}
		}
	}

	std::sort(matchCounts.begin(), matchCounts.end());
	std::cout << "Results for " << ITERATION_COUNT << " random substrings of length " << NEEDLE_SIZE << std::endl;
	std::cout << "Match count (min/mean/max): " << matchCounts[0] << "/" << GetMean(matchCounts) << "/" << matchCounts[matchCounts.size() - 1] << std::endl;
	for (auto bd: benchmarkDatas) {
		std::sort(bd.Times.begin(), bd.Times.end());
		std::cout << bd.AlgoName << " mean time: " << GetMean(bd.Times) << "ms" << std::endl;
	}
}
