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
		this->_db = rhs._db;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

static bool check_date(std::string const& line) {
	static const int DDELIM_ONE = 4, DDELIM_TWO = 7;
	if (line[DDELIM_ONE] != '-' || line[DDELIM_TWO] != '-') {
		throw BitcoinExchange::InvalidDateDelimiter();
	}
	std::string linedate = line;
	linedate.erase(DDELIM_ONE, 1).erase(DDELIM_TWO - 1, 1);
	const std::string year = linedate.substr(0, 4);
	const std::string day  = linedate.substr(linedate.length() - 2, 2);
	if (linedate.find_first_not_of(DIGITS) != std::string::npos || year.length() != 4) {
		throw BitcoinExchange::InvalidDateFormat();
	}
	const int        month_idx         = std::atoi((linedate.substr(4, 2).c_str()));
	const int        day_idx           = std::atoi(day.c_str());
	static const int MONTHS_LENGTH[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static const int MONTHS_YEAR       = 12;
	if (month_idx < 1 || month_idx > MONTHS_YEAR) {
		throw BitcoinExchange::InvalidDateMonths();
	}
	if (day_idx < 1 || day_idx > MONTHS_LENGTH[month_idx - 1]) {
		throw BitcoinExchange::InvalidDateDays();
	}
	if (month_idx == 2 && day_idx == MONTHS_LENGTH[month_idx]
		&& std::atoi(year.c_str()) % 4 != 0) {
		throw BitcoinExchange::InvalidDateDays();
	}
	return true;
}

static bool check_value(std::string const& lineval) {
	if (lineval.length() == 0) {
		throw std::out_of_range("");
	}
	if (lineval.find("-") != std::string::npos) {
		throw BitcoinExchange::NegativeValueException();
	}
	if (lineval.find_first_not_of(DIGITS ".") != std::string::npos) {
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
std::pair<std::string, double>
BitcoinExchange::get_date_value_input(std::string const& line) {
	const std::string date  = line.substr(0, DATE_LENGTH);
	const std::string value = line.substr(BEGIN_VALUE);
	check_date(date);
	if (line.find(" | ") == std::string::npos || line.substr(DATE_LENGTH, 3) != " | ") {
		throw BitcoinExchange::InvalidFormattingException();
	}
	check_value(value);
	if (std::atof(value.c_str()) > MAX_INPUT_VALUE) {
		throw BitcoinExchange::TooLargeValueException();
	}
	return std::make_pair(date, std::atof(value.c_str()));
}

static std::pair<std::string, double> get_date_value_db(std::string const& dbline) {
	const std::string date  = dbline.substr(0, DATE_LENGTH);
	const std::string value = dbline.substr(DATE_LENGTH + 1);
	check_date(date);
	if (value.length() == 0) {
		throw std::out_of_range("");
	}
	if (dbline.find(",") == std::string::npos || dbline[DATE_LENGTH] != ',') {
		throw BitcoinExchange::InvalidFormattingException();
	}
	check_value(value);
	return std::make_pair(date, std::strtod(value.c_str(), (char**)NULL));
}

/**
 * @brief parse data.csv into dbhashmap (_db)
 * 
 */
void BitcoinExchange::create_db() {
	std::ifstream dbfile("data.csv");
	std::string   dbline;
	if (!dbfile.is_open()) {
		throw DBCreationException();
	}
	if (!std::getline(dbfile, dbline) || dbline != "date,exchange_rate") {
		dbfile.close();
		throw DBCreationException();
	}
	for (; std::getline(dbfile, dbline);) {
		if (dbline != "date,exchange_rate" && !dbline.empty()
			&& dbline.find_first_not_of(DIGITS ",.-") == std::string::npos) {
			_db.insert(get_date_value_db(dbline));
		} else if (dbline.find_first_not_of(DIGITS ",.-") != std::string::npos) {
			dbfile.close();
			throw DBCreationException();
		}
	}
	dbfile.close();
}

/**
 * @brief find date in _db (lower date if not found)
 * 
 * @param date_value key value pair
 */
void BitcoinExchange::runExchange(std::pair<std::string, double> date_value) {
	for (std::map<std::string, double>::iterator it = _db.begin(); it != _db.end();
		 ++it) {
		if (it->first >= date_value.first) {
			// walk back to find the closest date
			for (; it->first > date_value.first && it != _db.begin(); --it) {
			}
			// std::cout << "date_value.first: '" << date_value.first
			// 		  << "' db_date: '" << it->first << "'" << std::endl;
			// in place calculate value * exchange_rate
			double exchange_rate = it->second * date_value.second;
			// print in format "date => value = exchange_rate"
			std::cout << date_value.first << " => " << date_value.second << " = "
					  << exchange_rate << std::endl;
			break;
		}
	}
}

/**
 * @brief load data.csv, read input file and validate, run date comparison, calculate rates
 * 
 */
BitcoinExchange::BitcoinExchange(std::ifstream& infile) {
	try {
		create_db();
		std::string line;
		if (std::getline(infile, line) && line != "date | value") {
			std::cerr << "Error: invalid input file." << std::endl;
		}
	} catch (DBCreationException& e) {
		infile.close();
		std::cerr << e.what() << std::endl;
		return;
	}
	for (std::string line; std::getline(infile, line);) {
		try {
			std::pair<std::string, double> date_value = get_date_value_input(line);
			runExchange(date_value);
		} catch (NegativeValueException& e) {
			std::cerr << e.what() << std::endl;
		} catch (TooLargeValueException& e) {
			std::cerr << e.what() << std::endl;
		} catch (std::exception& e) {
			std::cerr << "Error: bad input => " << line << std::endl;
		}
	}
	infile.close();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */