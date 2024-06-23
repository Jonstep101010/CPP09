#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PmergeMe::PmergeMe()
	: unpaired(0) {}

PmergeMe::PmergeMe(const PmergeMe& src)
	: unpaired(0) {
	*this = src;
}

PmergeMe::PmergeMe(char** argv)
	: unpaired(0) {
	while (*++argv) {
		std::string str(*argv);
		if (str.find_first_not_of("0123456789") != std::string::npos) {
			std::cerr << "Error" << std::endl;
			exit(1);
		}
		int num = std::atoi((const char*)*argv);
		// we only check the vector for duplicates (same data, array access is faster)
		onlyUniqueVec(num);
		// push to both containers
		numbers_vec.push_back(num);
		numbers_deq.push_back(num);
	}
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

PmergeMe::~PmergeMe() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

PmergeMe& PmergeMe::operator=(PmergeMe const& rhs) {
	if (this != &rhs) {
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void PmergeMe::set_jacobsthal(size_t size) {
	jacobsthal.clear();
	if (size >= 2) {
		jacobsthal.push_back(0);
		jacobsthal.push_back(1);
		for (size_t i = 2; jacobsthal.back() < (int)(size / 2 + 1); i++) {
			int nextNumber = jacobsthal[i - 1] + 2 * jacobsthal[i - 2];
			jacobsthal.push_back(nextNumber);
		}
	} else {
		jacobsthal.push_back(0);
	}
}

/**
 * @brief check for duplicates to prevent issues in sorting
 */
void PmergeMe::onlyUniqueVec(int num) {
	if (std::find(numbers_vec.begin(), numbers_vec.end(), num) != numbers_vec.end()) {
		std::cerr << "Error" << std::endl;
		exit(1);
	}
}

static void printVector(std::vector<int> const& vec) {
	std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
}

void PmergeMe::createPairs() {
	if (numbers_vec.size() % 2 != 0) {
		unpaired = numbers_vec.back();
		numbers_vec.pop_back();
		numbers_deq.pop_back();
		std::cout << "Unpaired: " << unpaired << std::endl;
	}
	// group into size / 2 pairs
	for (size_t i = 0; i < numbers_vec.size(); i += 2) {
		pairs.push_back(std::make_pair(numbers_vec[i], numbers_vec[i + 1]));
	}
	std::cout << "Pairs: ";
	// clang-format off
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin();
		 // clang-format on
		 it != pairs.end(); ++it) {
		std::cout << "[" << it->first << " " << it->second << "] ";
	}
	std::cout << std::endl;
}

void PmergeMe::unsortEachPair() {
	// clang-format off
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin();
	// clang-format on	
		 it != pairs.end(); ++it) {
		if (it->first < it->second) {
			std::swap(it->first, it->second);
		}
	}
	std::cout << "Unsorted: ";
	// clang-format off
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin();
		 // clang-format on
		 it != pairs.end(); ++it) {
		std::cout << "[" << it->first << " " << it->second << "] ";
	}
}

void PmergeMe::sort() {
	// requires parsed input
	// Print before
	std::cout << "Before: ";
	printVector(numbers_vec);
	createPairs();
	unsortEachPair();
	set_jacobsthal(numbers_vec.size());
	// std::cout << "Jacobsthal: ";
	// printVector(jacobsthal);
	// start timer 1
	// Sort 1
	// end timer 1
	// start timer 2
	// Sort 2
	// end timer 2
	// Print after
	std::cout << "After: ";
	printVector(numbers_vec);
	// Print time 1
	// Print time 2
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */