#pragma once

#include <map>
#include <string>

#define DIGITS "0123456789"
#define BEGIN_VALUE 13
#define DATE_LENGTH 10
#define MAX_INPUT_VALUE 1000

class BitcoinExchange {

public:
	BitcoinExchange();
	BitcoinExchange(std::ifstream& infile);
	BitcoinExchange(BitcoinExchange const& src);
	~BitcoinExchange();

	BitcoinExchange& operator=(BitcoinExchange const& rhs);

	std::map<std::string, double> _db;

	static std::pair<std::string, double> get_date_value_input(std::string const& line);
	void                                  create_db();
	void runExchange(std::pair<std::string, double> date_value);

	class DBCreationException : public std::exception {
	public:
		const char* what() const throw() { return "Error: invalid data.csv"; }
	};
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
	public:
		const char* what() const throw() { return "Invalid value."; }
	};
	class NegativeValueException : public InvalidValueException {
	public:
		const char* what() const throw() { return "Error: not a positive number."; }
	};
	class TooLargeValueException : public InvalidValueException {
	public:
		const char* what() const throw() { return "Error: too large a number."; }
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