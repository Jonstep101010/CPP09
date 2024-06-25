#pragma once
#include <deque>
#include <iostream>
#include <string>
#include <vector>

class PmergeMe {

public:
	PmergeMe();
	PmergeMe(PmergeMe const& src);
	PmergeMe(char** argv);
	~PmergeMe();

	PmergeMe& operator=(PmergeMe const& rhs);

	void sort();

	class Error : public std::exception {
		virtual const char* what() const throw() { return "Error"; }
	};

private:
	size_t size;
	// heap-array (contiguous memory)
	// insertions/deletions at beginning/middle slower than deque (re-allocation, copying)
	std::vector<int> numbers_vec;
	// double ended queue (non-contiguous memory)
	// indexing slower than vector, but faster insertion/deletion at ends
	std::deque<int> numbers_deq;

	// bool	is_odd; indicated by size
	// size_t size;
	int unpaired;

	// clang-format off
	std::vector<std::pair<int, int> > pairs;
	// clang-format on
	std::vector<int> main_chain;
	std::vector<int> pend;

	std::vector<int> jacobsthal;

	void set_jacobsthal(size_t size);

	// @follow-up use std::pair for sorting?

	void onlyUniqueVec(int num);
	void createPairs();
	void unsortEachPair();
	void sortPairsByFirst();
	void collectPairs();
	void insertionSort();

	// clang-format off
	std::vector<std::pair<int, int> >::iterator findLargest_range(std::vector<std::pair<int, int> >::iterator start, std::vector<std::pair<int, int> >::iterator end);
	std::vector<std::pair<int, int> >::iterator findSmallest_range(std::vector<std::pair<int, int> >::iterator start, std::vector<std::pair<int, int> >::iterator end);
	// clang-format on
};
