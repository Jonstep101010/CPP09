#pragma once
#include <iostream>
#include <string>

class PmergeMe {

public:
	PmergeMe();
	PmergeMe(PmergeMe const& src);
	~PmergeMe();

	PmergeMe& operator=(PmergeMe const& rhs);

private:
};
