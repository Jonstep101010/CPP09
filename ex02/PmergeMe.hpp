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
	// heap-array (contiguous memory)
	// insertions/deletions at beginning/middle slower than deque (re-allocation, copying)
	std::vector<int> numbers_vec;
	// double ended queue (non-contiguous memory)
	// indexing slower than vector, but faster insertion/deletion at ends
	std::deque<int> numbers_deq;

	void onlyUniqueVec(int num);
};
