#pragma once
#include <iostream>
#include <iterator>

template <typename Container> void PmergeMe::printContainer(Container& c) {
	std::copy(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
}

template <typename Container>
void PmergeMe::printContainerName(Container& c, std::string name) {
	std::cout << name << ": [ ";
	std::copy(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "]" << std::endl;
}

template <typename PairsContainer> void PmergeMe::printContainerPairs(PairsContainer& c) {
	std::cout << "Pairs: ";
	// clang-format off
		for (typename PairsContainer::const_iterator it = c.begin();
		 // clang-format on
		 it != c.end(); ++it) {
		std::cout << "[" << it->first << " " << it->second << "] ";
	}
	std::cout << std::endl;
}