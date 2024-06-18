#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) { *this = src; }

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

BitcoinExchange::~BitcoinExchange() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

BitcoinExchange& BitcoinExchange::operator=(BitcoinExchange const& rhs) {
	if (this != &rhs) {
		this->db = rhs.db;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
#define DAYS                                                                             \
	(char[31][3]) {                                                                      \
		"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13",    \
			"14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25",      \
			"26", "27", "28", "29", "30", "31"                                           \
	}
#define MONTHS                                                                           \
	(char[12][3]) {                                                                      \
		"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"           \
	}

#define DIGITS "0123456789"

// @todo handle month lengths & leap years...
static bool check_months(std::string month) {
	for (int i = 0; i < 12; i++) {
		if (month == MONTHS[i]) {
			return true;
		}
	}
	return false;
}

// @todo handle month lengths & leap years...
static bool check_days(std::string day) {
	for (int i = 0; i < 31; i++) {
		if (day == DAYS[i]) {
			return true;
		}
	}
	return false;
}

// @todo
static bool check_date(std::string linedate) {
	// @follow-up create more specific exceptions
	if (linedate.length() != 10) {
		throw BitcoinExchange::InvalidDateException();
	}
	if (linedate[4] != '-' || linedate[7] != '-') {
		// make sure index 4 and 7 are '-'
		throw BitcoinExchange::InvalidDateDelimiter();
	}
	if (linedate.find_first_not_of("0123456789-") != std::string::npos
		|| linedate.substr(5, 2).find_first_not_of(DIGITS) != std::string::npos) {
		throw BitcoinExchange::InvalidDateFormat();
	}
	if (linedate.substr(0, 4).find_first_not_of(DIGITS) != std::string::npos) {
		// make sure year is between 0000 and 9999
		throw BitcoinExchange::InvalidDateYears();
	}
	if (!check_months(linedate.substr(5, 2))) {
		throw BitcoinExchange::InvalidDateMonths();
	}
	if (!check_days(linedate.substr(8, 2))) {
		throw BitcoinExchange::InvalidDateDays();
	}
	// @todo handle edge cases in months and their max days
	// @todo handle leap years
	return true;
}

// @todo implement error handling
static bool check_value(std::string lineval) {
	if (std::atof(lineval.c_str()) > 1000
		|| lineval.find_first_not_of("0123456789.") != std::string::npos) {
		throw BitcoinExchange::InvalidValueException();
	}
	return true;
}

/**
 * @brief validate input
 * 
 * @param line 
 * @return std::pair<std::string, double> 
 */
std::pair<std::string, double> BitcoinExchange::get_date_value(std::string line) {
	if (line == "date | value") {
		return std::pair<std::string, double>("", 0);
	}
	// try {
	if (line.find(" | ") == std::string::npos) {
		// line[10,11,12] = " | "
		throw BitcoinExchange::MissingSeparatorException();
	}
	std::string date = line.substr(0, 10);
	// length has to be >= 13
	std::string value = line.substr(13);
	if (date.length() != 10 || value.length() == 0) {
		throw BitcoinExchange::LineTooShortException();
	}
	if (value.find_first_not_of("0123456789.") != std::string::npos) {
		throw BitcoinExchange::InvalidValueException();
	}
	check_date(date);
	check_value(value);
	// } catch (std::exception& e) {
	// @follow-up do actual handling (exception, return, etc.)
	// }

	// create split of date | value => std::pair<date, value>
	return std::pair<std::string, double>("", 0);
}

BitcoinExchange::BitcoinExchange(std::ifstream& db_filename) {
	// parse internal data.csv...

	// read input file and validate, parse into dbhashmap
	for (std::string line; std::getline(db_filename, line);) {
		std::cout << line << std::endl;
		// create split of date | value => dbhashmap[date] = value
		db.insert(get_date_value(line));
	}
	db_filename.close();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */