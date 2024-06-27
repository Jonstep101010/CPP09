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

// check if main_deq is sorted & no duplicates @audit remove
static void assertMainSortedDeq(std::deque<int> inputdeq, int unpaired, size_t size,
								std::deque<int> main_deq) {
	if (size % 2 != 0) {
		inputdeq.push_back(unpaired);
	}
	std::deque<int> sorted_chain = inputdeq;
	std::sort(sorted_chain.begin(), sorted_chain.end());
	if (main_deq != sorted_chain) {
		std::cerr << "Error\nSorted != main\nsorted_chain:\n";
		std::copy(sorted_chain.begin(), sorted_chain.end(),
				  std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::cerr << "main_deq\n";
		std::copy(main_deq.begin(), main_deq.end(),
				  std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		exit(1);
	}
	for (std::deque<int>::iterator it = main_deq.begin(); it != main_deq.end(); ++it) {
		if (std::find(it + 1, main_deq.end(), *it) != main_deq.end()) {
			std::cerr << "Error" << std::endl;
			exit(1);
		}
	}
}

#include <fstream>
#define ERROR(x) std::cerr << "\033[1;31m" << x << "\033[0m"
#define OK(x) std::cout << "\033[1;32m" << x << "\033[0m"
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

	// Sort 1
	insertionSort<std::vector<int> >(main_vec, pend_vec, jthal_vec, size);
	assertMainSortedVec(numbers_vec, unpaired, size, main_vec);
	// clang-format on
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
	// Sort 2
	insertionSort<std::deque<int> >(main_deq, pend_deq, jthal_deq, size);
	assertMainSortedDeq(numbers_deq, unpaired, size, main_deq);
	// clang-format on
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
	compare_sorted(); // @audit remove
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */