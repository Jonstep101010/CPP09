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

// @todo handle month lengths & leap years...
static bool get_day_idx(std::string day) {
	for (int i = 0; i < 31; i++) {
		if (day == DAYS[i]) {
			return i;
		}
	}
	throw BitcoinExchange::InvalidDateDays();
}

static bool check_date(std::string linedate) {
	if (linedate[4] != '-' || linedate[7] != '-') {
		// make sure index 4 and 7 are '-'
		throw BitcoinExchange::InvalidDateDelimiter();
	}
	const std::string year  = linedate.substr(0, 4);
	const std::string month = linedate.substr(5, 2);
	const std::string day   = linedate.substr(8, 2);
	if ((year + month + day).find_first_not_of(DIGITS) != std::string::npos) {
		throw BitcoinExchange::InvalidDateFormat();
	}
	const int month_idx = get_month_idx(month);
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
	// try {
	std::string date = line.substr(0, DATE_LENGTH);
	// length has to be >= 13
	std::string value = line.substr(BEGIN_VALUE);

	check_date(date);
	if (line.substr(DATE_LENGTH, 3) != " | ") {
		if (line.find(" | ") == std::string::npos) {
			throw BitcoinExchange::MissingSeparatorException();
		}
		throw BitcoinExchange::InvalidFormattingException();
	}
	if (value.length() == 0) {
		throw BitcoinExchange::LineTooShortException();
	}
	if (value.find_first_not_of("0123456789.") != std::string::npos) {
		throw BitcoinExchange::InvalidValueException();
	}
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