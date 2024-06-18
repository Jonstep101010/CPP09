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

#define MONTHS_LENGTH                                                                    \
	(int[12]) { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }

#define DIGITS "0123456789"

#define BEGIN_VALUE 13
#define DATE_LENGTH 10

static int get_month_idx(std::string month) {
	for (int i = 0; i < 12; i++) {
		if (month == MONTHS[i]) {
			return i;
		}
	}
	throw BitcoinExchange::InvalidDateMonths();
}

static bool get_day_idx(std::string day) {
	for (int i = 0; i < 31; i++) {
		if (day == DAYS[i]) {
			return i;
		}
	}
	throw BitcoinExchange::InvalidDateDays();
}

static bool check_date(std::string linedate) {
	std::cout << "check_date: " << linedate << std::endl;
	const std::string year = linedate.substr(0, 4);
	const std::string day  = linedate.substr(linedate.length() - 2, 2);
	std::cout << "year: " << year << std::endl;
	// std::cout << "day: " << day << std::endl;
	if (linedate.find_first_not_of(DIGITS) != std::string::npos || year.length() != 4) {
		throw BitcoinExchange::InvalidDateFormat();
	}
	const int month_idx = get_month_idx(linedate.substr(4, 2));
	const int day_idx   = get_day_idx(day);
	// handle max days, including leap years
	if (day_idx > MONTHS_LENGTH[month_idx]
		|| (month_idx == 2 && day_idx == 29 && std::atoi(year.c_str()) % 4 != 0)) {
		throw BitcoinExchange::InvalidDateDays();
	}
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
	if (line[4] != '-' || line[7] != '-') {
		throw BitcoinExchange::InvalidDateDelimiter();
	}
	// @follow-up do actual handling (exception, return, etc.)
	try {
		std::string date  = line.substr(0, DATE_LENGTH);
		std::string value = line.substr(BEGIN_VALUE);
		check_date(date.erase(4, 1).erase(6, 1));
		if (line.find(" | ") == std::string::npos) {
			throw BitcoinExchange::MissingSeparatorException();
		}
		if (line.substr(DATE_LENGTH, 3) != " | ") {
			throw BitcoinExchange::InvalidFormattingException();
		}
		if (value.length() == 0) {
			throw BitcoinExchange::LineTooShortException();
		}
		if (value.find_first_not_of("0123456789.") != std::string::npos) {
			throw BitcoinExchange::NegativeValueException();
		}
		check_value(value);
	} catch (std::out_of_range& e) {
		std::cerr << "Error: bad input => " << line << std::endl;
	} catch (NegativeValueException& e) {
		std::cerr << e.what() << std::endl;
	}

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