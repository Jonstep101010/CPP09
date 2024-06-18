#pragma once

#include <map>
#include <string>

class BitcoinExchange {

public:
	BitcoinExchange();
	BitcoinExchange(std::ifstream& db_filename);
	BitcoinExchange(BitcoinExchange const& src);
	~BitcoinExchange();

	BitcoinExchange& operator=(BitcoinExchange const& rhs);

	std::map<std::string, double> db;

	std::pair<std::string, double> get_date_value(std::string line);

	class InvalidDateException : public std::exception {
		const char* what() const throw() { return "Invalid date."; }
	};
	class InvalidDateDelimiter : public InvalidDateException {
	public:
		const char* what() const throw() { return "Requires '-' as delimiter."; }
	};
	class InvalidDateYears : public InvalidDateException {
	public:
		const char* what() const throw() {
			return "Valid years are between 0000 and 9999.";
		}
	};
	class InvalidDateMonths : public InvalidDateException {
	public:
		const char* what() const throw() {
			return "Month has to be in between 01 and 12";
		}
	};
	class InvalidDateDays : public InvalidDateException {
	public:
		const char* what() const throw() { return "Day is invalid in this context."; }
	};
	class InvalidDateFormat : public InvalidDateException {
	public:
		const char* what() const throw() { return "only \"0123456789-\" are permitted!"; }
	};
	class InvalidValueException : public std::exception {
		const char* what() const throw() { return "Invalid value."; }
	};
	class InvalidLineException : public std::exception {
		const char* what() const throw() { return "Invalid line."; }
	};
	class MissingSeparatorException : public InvalidLineException {
		const char* what() const throw() {
			return "Missing separator of format \" | \".";
		}
	};
	class InvalidFormattingException : public InvalidLineException {
		const char* what() const throw() {
			return "Required format: \"YYYY-MM-DD | value\".";
		}
	};
	class LineTooShortException : public InvalidLineException {
	public:
		const char* what() const throw() { return "Line too short."; }
	};

private:
};