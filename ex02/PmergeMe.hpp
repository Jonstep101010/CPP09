#pragma once
#include <cstddef>
#include <ctime>
#include <deque>
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
	int    unpaired;

	clock_t start;
	char**  argv;

	double timeElapsedVec;
	double timeElapsedDeq;

	/// VECTOR ///
	// heap-array (contiguous memory)
	// insertions/deletions at beginning/middle slower than deque (re-allocation, copying)
	std::vector<int> numbers_vec;
	// clang-format off
	std::vector<std::pair<int, int> > pairs_vec;
	// clang-format on
	std::vector<int> main_vec;
	std::vector<int> pend_vec;
	std::vector<int> jthal_vec;

	/// DEQUE ///
	// double ended queue (non-contiguous memory)
	// indexing slower than vector, but faster insertion/deletion at ends
	std::deque<int> numbers_deq;
	// clang-format off
	std::deque<std::pair<int, int> > pairs_deq;
	// clang-format on

	std::deque<int> main_deq;
	std::deque<int> pend_deq;
	std::deque<int> jthal_deq;

	/* --------------------------------- METHODS ---------------------------------- */

	void insertionSortVector();
	void insertionSortDeque();

	void debugPrintSortVec(size_t i);

	/* -------------------------------- Templates ------------------------------- */
	template <typename Container> void printContainer(Container& c);
	template <typename Container> void printContainerName(Container& c, std::string name);
	template <typename PairsContainer> void printContainerPairs(PairsContainer& c);

	template <typename Container> void set_jacobsthal(size_t size, Container& jacobsthal);

	template <typename Container> void get_input(Container& numbers);

	/// PAIRS OPS ///
	template <typename Container, typename PairsContainer>
	void createPairs(Container& numbers, PairsContainer& pairs);
	template <typename PairsContainer> void unsortEachPair(PairsContainer& pairs);

	template <typename PairsContainer> void sortPairsByFirst(PairsContainer& pairs);

	template <typename Container, typename PairsContainer>
	void collectPairs(PairsContainer& pairs, Container& main, Container& pend);
};
