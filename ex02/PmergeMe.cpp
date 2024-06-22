#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& src) { *this = src; }

PmergeMe::PmergeMe(char** argv) {
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

void PmergeMe::sort() {
	// requires parsed input
	// Print before
	std::cout << "Before: ";
	printVector(numbers_vec);
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