#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cctype>
#include <cstring>
#include <ctime>

#include "algorithms.h"

typedef void (*TMatcher)(const std::string& needle, const std::string& haystack, std::vector<size_t>& answer);

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

double MeasureTime(const std::string& matcherName, const TMatcher& matcher, const std::string& needle, const std::string& haystack, std::vector<size_t>& result) {
	clock_t start = clock();
	matcher(needle, haystack, result);
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
		{ "bruteforce", MatchNaively },
		{ "KMP", MatchWithKMP },
	};

	std::vector<size_t> matchCounts;
	for (size_t iter = 0; iter < ITERATION_COUNT; iter++) {
		auto randomPos = rand() % haystack.length();
		std::string needle(haystack.substr(randomPos, NEEDLE_SIZE));

		std::vector<size_t> correctResult;
		MatchNaively(needle, haystack, correctResult);
		matchCounts.push_back(correctResult.size());

		for (auto& bd: benchmarkDatas) {
			std::vector<size_t> result;
			auto elapsed = MeasureTime(bd.AlgoName, bd.Matcher, needle, haystack, result);
			bd.Times.push_back(elapsed);
			if (!std::equal(correctResult.begin(), correctResult.end(), result.begin())) {
				throw std::runtime_error(bd.AlgoName + " gives incorrect results");
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
