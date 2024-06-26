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

