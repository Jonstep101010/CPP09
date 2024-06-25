#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

static void printVectorName(std::vector<int> const& vec, std::string vecname);

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PmergeMe::PmergeMe()
	: size(0), unpaired(0) {}

PmergeMe::PmergeMe(const PmergeMe& src)
	: size(0), unpaired(0) {
	*this = src;
}

PmergeMe::PmergeMe(char** argv)
	: size(0), unpaired(0) {
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
	size = numbers_vec.size();
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
	printVectorName(jacobsthal, "Jacobsthal");
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

static void printVectorName(std::vector<int> const& vec, std::string vecname) {
	std::cout << vecname << ": [ ";
	std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "]" << std::endl;
}

// clang-format off
static void printPairsVec(std::vector<std::pair<int, int> > const& vec) {
	std::cout << "Pairs: ";
	for (std::vector<std::pair<int, int> >::const_iterator it = vec.begin();
		 // clang-format on
		 it != vec.end(); ++it) {
		std::cout << "[" << it->first << " " << it->second << "] ";
	}
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
	printPairsVec(pairs);
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
	std::cout << "Unsorted ";
	printPairsVec(pairs);
}

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

void PmergeMe::sortPairsByFirst() {
	size_t sizeloc = pairs.size();
	// start from end

	// clang-format off
	std::vector<std::pair<int, int> >::iterator largest = findLargest_range(pairs.begin(), pairs.end());
	for (std::vector<std::pair<int, int> >::iterator it = pairs.end() - 1;
		 // clang-format on
		 it != pairs.begin(); --it) {
		largest = findLargest_range(pairs.begin(), it);
		std::swap(*it, *largest);
	}
	// prevent skipping first element
	// clang-format off
	std::vector<std::pair<int, int> >::iterator smallest = findSmallest_range(pairs.begin(), pairs.end());
	for (std::vector<std::pair<int, int> >::iterator it = pairs.begin();
		 // clang-format on
		 it != pairs.end(); ++it) {
		smallest = findSmallest_range(it, pairs.end());
		std::swap(*it, *smallest);
	}

	if (sizeloc != pairs.size()) {
		std::cerr << "sortPairsByFirst panicked!" << std::endl;
		abort();
	}

	std::cout << "Sorted ";
	printPairsVec(pairs);
}

void PmergeMe::collectPairs() {
	if (pairs[0].second < pairs[0].first && pairs.size() > 1) {
		main_chain.push_back(pairs[0].second);
		main_chain.push_back(pairs[0].first);
		pairs.erase(pairs.begin());
	}
	for (size_t i = 0; i < pairs.size(); i++) {
		main_chain.push_back(pairs[i].first);
		pend.push_back(pairs[i].second);
	}
	// print main_chain and pend
	printVectorName(main_chain, "main_chain");
	printVectorName(pend, "pend");
}

void PmergeMe::insertionSort() {
	// make use of jacobsthal
	set_jacobsthal(pend.size());
	std::vector<int>::iterator upper;
	for (size_t i = 0; i < jacobsthal.size() && !pend.empty(); ++i) {
		const int& val = pend.size() == 1 ? pend[0] : pend[jacobsthal[i]];
		upper          = std::upper_bound(main_chain.begin(), main_chain.end(), val);
		std::cout << "inserting pend elem " << val << " at upper: " << *upper
				  << std::endl;
		main_chain.insert(upper, val);
		pend.erase(pend.begin() + jacobsthal[i]);
		printVectorName(main_chain, "main_chain");
		printVectorName(pend, "pend");
	}
	pend.erase(pend.begin());
	while (!pend.empty()) {
		upper = std::upper_bound(main_chain.begin(), main_chain.end(), pend[0]);
		std::cout << "inserting pend elem " << pend[0] << " at upper: " << *upper
				  << std::endl;
		if (std::find(main_chain.begin(), main_chain.end(), pend[0])
			!= main_chain.end()) {
			std::cerr << "Error" << std::endl;
			exit(1);
		}
		main_chain.insert(upper, pend[0]);
		pend.erase(pend.begin());
		printVectorName(main_chain, "main_chain");
		printVectorName(pend, "pend");
	}
	// handle unpaired
	if (size % 2 != 0) {
		std::vector<int>::iterator upper
			= std::upper_bound(main_chain.begin(), main_chain.end(), unpaired);
		main_chain.insert(upper, unpaired);
	}
	// check if main_chain is sorted & no duplicates @audit remove
	std::vector<int> sorted_chain = main_chain;
	std::sort(sorted_chain.begin(), sorted_chain.end());
	if (main_chain != sorted_chain) {
		std::cerr << "Error" << std::endl;
		exit(1);
	}
	for (std::vector<int>::iterator it = main_chain.begin(); it != main_chain.end();
		 ++it) {
		if (std::find(it + 1, main_chain.end(), *it) != main_chain.end()) {
			std::cerr << "Error" << std::endl;
			exit(1);
		}
	}
}

void PmergeMe::sort() {
	// requires parsed input
	// Print before
	std::cout << "Before: ";
	printVector(numbers_vec);
	createPairs();
	unsortEachPair();
	sortPairsByFirst();
	collectPairs();
	// run insertion algo
	insertionSort();

	// start timer 1
	// Sort 1
	// end timer 1
	// start timer 2
	// Sort 2
	// end timer 2
	// Print after
	std::cout << "After: ";
	printVector(main_chain);
	// Print time 1
	// Print time 2
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */