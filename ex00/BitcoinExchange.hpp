#pragma once

#include <iostream>
#include <map>
#include <string>

class BitcoinExchange {

public:
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange const& src);
	~BitcoinExchange();

	BitcoinExchange& operator=(BitcoinExchange const& rhs);

	std::map<std::string, double> db;

	bool init(std::string db_filename);

private:
};

// std::ostream& operator<<(std::ostream& o, BitcoinExchange const& i);