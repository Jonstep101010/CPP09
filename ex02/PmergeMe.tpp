#pragma once
#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>

#define PRINTDEF 1

template <typename Container> void PmergeMe::printContainer(Container& c) {
#if PRINTDEF
	std::copy(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
#else
	(void)c;
#endif
}

template <typename Container>
void PmergeMe::printContainerName(Container& c, std::string name) {
#if PRINTDEF
	std::cout << name << ": [ ";
	std::copy(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "]" << std::endl;
#else
	(void)c;
	(void)name;
#endif
}

template <typename PairsContainer> void PmergeMe::printContainerPairs(PairsContainer& c) {
#if PRINTDEF
	std::cout << "Pairs: ";
	for (typename PairsContainer::const_iterator it = c.begin(); it != c.end(); ++it) {
		std::cout << "[" << it->first << " " << it->second << "] ";
	}
	std::cout << std::endl;
#else
	(void)c;
#endif
}

template <typename Container>
void PmergeMe::set_jacobsthal(size_t size, Container& jacobsthal) {
	jacobsthal.clear();
	if (size >= 2) {
		jacobsthal.push_back(0);
		jacobsthal.push_back(1);
		for (size_t i = 2; jacobsthal.back() < (int)(size / 2 + 1); i++) {
			int nextNumber = jacobsthal[i - 1] + 2 * jacobsthal[i - 2];
			jacobsthal.push_back(nextNumber);
		}
		jacobsthal.erase(jacobsthal.begin() + 1);
	} else {
		jacobsthal.push_back(0);
	}
	printContainerName(jacobsthal, "Jacobsthal");
}

#include "PmergeMe.hpp"
template <typename Container> void PmergeMe::get_input(Container& numbers) {
	if (!argv) {
		throw Error();
	}
	for (int i = 1; argv[i]; i++) {
		const std::string str(argv[i]);
		if (str.find_first_not_of("0123456789") != std::string::npos) {
			std::cerr << "Error" << std::endl;
			exit(1);
		}
		const int num = std::atoi((const char*)argv[i]);
		if (std::find(numbers.begin(), numbers.end(), num) != numbers.end()) {
			throw Error();
		}
		// push to numbers container
		numbers.push_back(num);
	}
	size = numbers.size();
}

template <typename Container, typename PairsContainer>
void PmergeMe::createPairs(Container& numbers, PairsContainer& pairs) {
	if (numbers.size() % 2 != 0) {
		unpaired = numbers.back();
		numbers.pop_back();
		std::cout << "Unpaired: " << unpaired << std::endl;
	}
	// group into size / 2 pairs
	for (size_t i = 0; i < numbers.size(); i += 2) {
		pairs.push_back(std::make_pair(numbers[i], numbers[i + 1]));
	}
	printContainerPairs(pairs);
}

template <typename PairsContainer> void PmergeMe::unsortEachPair(PairsContainer& pairs) {
	for (typename PairsContainer::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		if (it->first < it->second) {
			std::swap(it->first, it->second);
		}
	}
	std::cout << "Unsorted ";
	printContainerPairs(pairs);
}

template <typename PairsContainer, typename Compare>
static typename PairsContainer::iterator
findCompareRange(typename PairsContainer::iterator start,
				 typename PairsContainer::iterator end) {
	typename PairsContainer::iterator target = start;
	Compare                           comp;

	for (typename PairsContainer::iterator it = start; it != end; ++it) {
		if (comp(it->first, target->first)) {
			target = it;
		}
	}
	return target;
}

template <typename PairsContainer>
void PmergeMe::sortPairsByFirst(PairsContainer& pairs) {
	typename PairsContainer::iterator largest
		// clang-format off
		= findCompareRange<PairsContainer, std::greater<int> >(pairs.begin(), pairs.end());
	for (typename PairsContainer::iterator it = pairs.end() - 1; it != pairs.begin();
		 --it) {
		largest = findCompareRange<PairsContainer, std::greater<int> >(pairs.begin(), it);
		std::swap(*it, *largest);
	}
	typename PairsContainer::iterator smallest
		= findCompareRange<PairsContainer, std::less<int> >(pairs.begin(), pairs.end());
	for (typename PairsContainer::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		smallest = findCompareRange<PairsContainer, std::less<int> >(it, pairs.end());
		// clang-format on
		std::swap(*it, *smallest);
	}
	std::cout << "Sorted ";
	printContainerPairs(pairs);
}

template <typename Container, typename PairsContainer>
void PmergeMe::collectPairs(PairsContainer& pairs, Container& main, Container& pend) {
	for (size_t i = 0; i < pairs.size(); i++) {
		main.push_back(pairs[i].first);
		pend.push_back(pairs[i].second);
	}
	// print main & pend
	printContainerName(main, "main");
	printContainerName(pend, "pend");
}