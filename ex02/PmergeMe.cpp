#include "PmergeMe.hpp"
#include "PmergeMe.tpp"
#include <algorithm>
#include <cstdlib>
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PmergeMe::PmergeMe()
	: size(0), unpaired(0), start(), argv(), timeElapsedVec(0), timeElapsedDeq(0) {}

PmergeMe::PmergeMe(const PmergeMe& src)
	: size(0), unpaired(0), start(), argv(), timeElapsedVec(0), timeElapsedDeq(0) {
	*this = src;
}

PmergeMe::PmergeMe(char** argv)
	: size(0), unpaired(0), start(), argv(argv), timeElapsedVec(0), timeElapsedDeq(0) {}

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

// check if main_vec is sorted & no duplicates @audit remove
static void assertMainSortedVec(std::vector<int> inputvec, int unpaired, size_t size,
								std::vector<int> main_vec) {
	if (size % 2 != 0) {
		inputvec.push_back(unpaired);
	}
	std::vector<int> sorted_chain = inputvec;
	std::sort(sorted_chain.begin(), sorted_chain.end());
	if (main_vec != sorted_chain) {
		std::cerr << "Error\nSorted != main\nsorted_chain:\n";
		std::copy(sorted_chain.begin(), sorted_chain.end(),
				  std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::cerr << "main_vec\n";
		std::copy(main_vec.begin(), main_vec.end(),
				  std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		exit(1);
	}
	for (std::vector<int>::iterator it = main_vec.begin(); it != main_vec.end(); ++it) {
		if (std::find(it + 1, main_vec.end(), *it) != main_vec.end()) {
			std::cerr << "Error" << std::endl;
			exit(1);
		}
	}
}

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
	assertMainSortedVec(numbers_vec, unpaired, size, main_vec);
}

void PmergeMe::insertionSortDeque() {
	// @todo implement insertion sort for deque
}

void PmergeMe::sort() {
	// requires parsed input @audit
	// start timer 1
	start = clock();
	get_input(numbers_vec);
	// Print before
	std::cout << "Before: ";
	printContainer(numbers_vec);
	createPairs(numbers_vec, pairs_vec);
	unsortEachPair(pairs_vec);
	sortPairsByFirst(pairs_vec);
	// clang-format off
	collectPairs<std::vector<int> >(pairs_vec, main_vec, pend_vec);
	// clang-format on

	// Sort 1
	insertionSortVector();
	// end timer 1
	timeElapsedVec = ((double)(clock() - start) / (CLOCKS_PER_SEC));

	// start timer 2
	start = clock();
	get_input(numbers_deq);
	createPairs(numbers_deq, pairs_deq);
	unsortEachPair(pairs_deq);
	sortPairsByFirst(pairs_deq);
	// clang-format off
	collectPairs<std::deque<int> >(pairs_deq, main_deq, pend_deq);
	// clang-format on
	// Sort 2
	insertionSortDeque();
	// end timer 2
	timeElapsedDeq = ((double)(clock() - start) / (CLOCKS_PER_SEC));
	// Print after
	std::cout << "After: \n";
	printContainerName(main_deq, "DEQUE");
	printContainerName(main_vec, "VECTOR");
	// Print time 1
	std::cout << "Time to process a range of " << size
			  << " elements with std::vector : " << timeElapsedVec << std::endl;
	// Print time 2
	std::cout << "Time to process a range of " << size
			  << " elements with std::deque : " << timeElapsedDeq << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */