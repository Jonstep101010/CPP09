#include "PmergeMe.hpp"
#include "PmergeMe.tpp"
#include <algorithm>
#include <cstdlib>
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PmergeMe::PmergeMe()
	: size(0), unpaired(0), start(), end(), argv() {}

PmergeMe::PmergeMe(const PmergeMe& src)
	: size(0), unpaired(0), start(), end(), argv() {
	*this = src;
}

PmergeMe::PmergeMe(char** argv)
	: size(0), unpaired(0), start(), end(), argv(argv) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

PmergeMe::~PmergeMe() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

PmergeMe& PmergeMe::operator=(PmergeMe const& rhs) {
	if (this != &rhs) {
		// @todo copy all data attributes
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// clang-format off
std::vector<std::pair<int, int> >::iterator PmergeMe::findLargest_range(std::vector<std::pair<int, int> >::iterator start, std::vector<std::pair<int, int> >::iterator end) {
	std::vector<std::pair<int, int> >::iterator largest = start;
	for (std::vector<std::pair<int, int> >::iterator it = start;
		 // clang-format on
		 it != end; ++it) {
		if (it->first > largest->first) {
			largest = it;
		}
	}
	// std::cout << "RangeLargest: " << largest->first << std::endl;
	return largest;
}
// clang-format off
std::vector<std::pair<int, int> >::iterator PmergeMe::findSmallest_range(std::vector<std::pair<int, int> >::iterator start, std::vector<std::pair<int, int> >::iterator end) {
	std::vector<std::pair<int, int> >::iterator smallest = start;
	for (std::vector<std::pair<int, int> >::iterator it = start;
		 // clang-format on
		 it != end; ++it) {
		if (it->first < smallest->first) {
			smallest = it;
		}
	}
	// std::cout << "RangeSmallest: " << smallest->first << std::endl;
	return smallest;
}

void PmergeMe::collectPairs() {
	for (size_t i = 0; i < pairs_vec.size(); i++) {
		main_vec.push_back(pairs_vec[i].first);
		pend_vec.push_back(pairs_vec[i].second);
	}
	// print main_vec and pend_vec
	printContainerName(main_vec, "main_vec");
	printContainerName(pend_vec, "pend_vec");
}

// check if main_vec is sorted & no duplicates @audit remove
// static void assertMainSorted(std::vector<int> inputvec, int unpaired, size_t size,
// 							 std::vector<int> main_vec) {
// 	if (size % 2 != 0) {
// 		inputvec.push_back(unpaired);
// 	}
// 	std::vector<int> sorted_chain = inputvec;
// 	std::sort(sorted_chain.begin(), sorted_chain.end());
// 	if (main_vec != sorted_chain) {
// 		std::cerr << "Error\nSorted != main\nsorted_chain:\n";
// 		printContainer(sorted_chain);
// 		std::cerr << "main_vec\n";
// 		printContainer(main_vec);
// 		exit(1);
// 	}
// 	for (std::vector<int>::iterator it = main_vec.begin(); it != main_vec.end();
// 		 ++it) {
// 		if (std::find(it + 1, main_vec.end(), *it) != main_vec.end()) {
// 			std::cerr << "Error" << std::endl;
// 			exit(1);
// 		}
// 	}
// }

void PmergeMe::debugPrintSortVec(size_t i) {
	std::cout << "jac: " << jthal_vec[i] << std::endl;
	std::cout << "val: "
			  << (i < jthal_vec.size() - 1 ? pend_vec[jthal_vec[i]] : pend_vec[0])
			  << std::endl;
	std::vector<int>::iterator upper = std::upper_bound(
		main_vec.begin(), main_vec.end(),
		(i < jthal_vec.size() - 1 ? pend_vec[jthal_vec[i]] : pend_vec[0]));
	std::cout << "inserting pend elem "
			  << (i < jthal_vec.size() - 1 ? pend_vec[jthal_vec[i]] : pend_vec[0])
			  << " at upper: " << *upper << std::endl;
}

void PmergeMe::insertionSortVector() {
	set_jacobsthal(pend_vec.size(), jthal_vec);
	std::vector<int>::iterator upper;
	for (size_t i = 0; !pend_vec.empty(); i++) {
		const int& val
			= (i < jthal_vec.size() - 1 ? pend_vec[jthal_vec[i]] : pend_vec[0]);
		upper = std::upper_bound(main_vec.begin(), main_vec.end(), val);
		debugPrintSortVec(i);
		main_vec.insert(upper, val);
		pend_vec.erase(pend_vec.begin() + (i < jthal_vec.size() - 1 ? jthal_vec[i] : 0));
		printContainerName(main_vec, "main_vec");
		printContainerName(pend_vec, "pend_vec");
	}
	// handle unpaired
	if (size % 2 != 0) {
		upper = std::upper_bound(main_vec.begin(), main_vec.end(), unpaired);
		main_vec.insert(upper, unpaired);
	}
	// assertMainSorted(numbers_vec, unpaired, size, main_vec);
}

void PmergeMe::insertionSortDeque() {
	// @todo implement insertion sort for deque
}

void PmergeMe::sort() {
	get_input(numbers_vec);
	// requires parsed input
	// Print before
	std::cout << "Before: ";
	printContainer(numbers_vec);
	// start timer 1
	start = clock();
	createPairs(numbers_vec, pairs_vec);
	unsortEachPair(pairs_vec);
	sortPairsByFirst(pairs_vec);
	collectPairs();

	// Sort 1
	insertionSortVector();
	// end timer 1
	end = clock();
	std::cout << "Time to process : " << ((double)(end - start) / (CLOCKS_PER_SEC / 1000))
			  << "ms\n";
	// start timer 2
	// Sort 2
	insertionSortDeque();
	// end timer 2
	// Print after
	std::cout << "After: ";
	printContainer(main_vec);
	// Print time 1
	// Print time 2
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */