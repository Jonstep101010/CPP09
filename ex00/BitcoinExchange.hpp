#pragma once

#include <iostream>
#include <string>

class BitcoinExchange {

public:
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange const& src);
	~BitcoinExchange();

	BitcoinExchange& operator=(BitcoinExchange const& rhs);

private:
};

// std::ostream& operator<<(std::ostream& o, BitcoinExchange const& i);