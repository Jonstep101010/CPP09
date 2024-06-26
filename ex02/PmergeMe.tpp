#pragma once
#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
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
