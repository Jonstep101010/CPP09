#include "PmergeMe.hpp"
#include "PmergeMe.tpp"
#include <algorithm>
#include <cstdlib>
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PmergeMe::PmergeMe()
	: argv(), unpaired(NULL), start(), timeElapsedVec(0), timeElapsedDeq(0) {}

PmergeMe::PmergeMe(const PmergeMe& src)
	: argv(), unpaired(NULL), start(), timeElapsedVec(0), timeElapsedDeq(0) {
	*this = src;
}

PmergeMe::PmergeMe(char** argv)
	: argv(argv), unpaired(NULL), start(), timeElapsedVec(0), timeElapsedDeq(0) {
	if (!argv) {
		throw Error();
	}
	std::cout << "Before: ";
	for (int i = 1; argv[i]; i++) {
		std::cout << argv[i] << " ";
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
		argv           = rhs.argv;
		unpaired       = rhs.unpaired;
		start          = rhs.start;
		timeElapsedVec = rhs.timeElapsedVec;
		timeElapsedDeq = rhs.timeElapsedDeq;
		numbers_vec    = rhs.numbers_vec;
		pairs_vec      = rhs.pairs_vec;
		main_vec       = rhs.main_vec;
		pend_vec       = rhs.pend_vec;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

#include <fstream>
void PmergeMe::compare_sorted() {
	system("rm -f outdeq.txt outvec.txt");
	// swap stdout to outfiles for diff
	std::ofstream outdeq("outdeq.txt");
	for (std::deque<int>::iterator it = main_deq.begin(); it != main_deq.end(); ++it) {
		outdeq << *it << " ";
	}
	outdeq.close();
	std::ofstream outvec("outvec.txt", std::ios::out);
	for (std::vector<int>::iterator it = main_vec.begin(); it != main_vec.end(); ++it) {
		outvec << *it << " ";
	}
	outvec.close();
	system("diff -s outdeq.txt outvec.txt") ? ERROR("Error\n") : OK("[OK]\n");
}

// @audit-info could be a template function
void PmergeMe::PrintSummary(std::vector<int>& vec) {
	std::cout << "After: ";
	std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "\nTime to process a range of " << numbers_vec.size()
			  << " elements with std::vector : " << timeElapsedVec
			  << "\nTime to process a range of " << numbers_deq.size()
			  << " elements with std::deque : " << timeElapsedDeq << std::endl;
}

// clang-format off
void PmergeMe::sort() {
	/* --------------------------------- VECTOR --------------------------------- */
	start = clock();
	get_input(numbers_vec);

	createPairs(numbers_vec, pairs_vec);
	unsortEachPair(pairs_vec);
	sortPairsByFirst(pairs_vec);
	collectPairs<std::vector<int> >(pairs_vec, main_vec, pend_vec);

	insertionSort<std::vector<int> >(main_vec, pend_vec, jthal_vec);
	timeElapsedVec = ((double)(clock() - start) / (CLOCKS_PER_SEC));
	// assertMainSorted<std::vector<int> >(numbers_vec, main_vec); // check if main_vec is sorted

	/* --------------------------------- DEQUE --------------------------------- */
	start = clock();
	get_input(numbers_deq);

	createPairs(numbers_deq, pairs_deq);
	unsortEachPair(pairs_deq);
	sortPairsByFirst(pairs_deq);

	collectPairs<std::deque<int> >(pairs_deq, main_deq, pend_deq);
	insertionSort<std::deque<int> >(main_deq, pend_deq, jthal_deq);
	timeElapsedDeq = ((double)(clock() - start) / (CLOCKS_PER_SEC));
	// assertMainSorted<std::deque<int> >(numbers_deq, main_deq); // check if main_deq is sorted

	/* -------------------------------------------------------------------------- */
	// compare_sorted(); // diff outdeq.txt outvec.txt
	PrintSummary(main_vec);
}
// clang-format on

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */