#pragma once

#include <iostream>
#include <vector>

#define DEBUG 1

template<typename T>
void DebugPrint(const std::string& name, const T& t) {
	std::cerr << name << ": " << t << std::endl;
}

template<typename T>
void DebugPrint(const std::string& name, const std::vector<T>& vector) {
	std::cerr << name << ":";
	for (const auto& item: vector) {
		std::cerr << " " << item;
	}
	std::cerr << std::endl;
}

#if DEBUG
#define DEBUG_PRINT(x) DebugPrint(#x, x)
#else
#define DEBUG_PRINT(x)
#endif