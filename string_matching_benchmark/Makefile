all:
	clang++ -O2 -g -std=c++11 algorithms.cpp main.cpp -o main

check:
	clang++ -O0 -g -std=c++11 algorithms.cpp tests.cpp -o tests && ./tests

time:
	for input in genome/ecoli.txt protein/hs.txt englishTexts/bible.txt rand16/rand16.txt; do \
		echo $$input; \
		echo =======; \
		./main data/$$input 4096 50; \
	done