#include "BitcoinExchange.hpp"

int main(int argc, char** argv) {
	if (argc == 2) {
		// handle file open error:
		// std::cerr << "Error: could not open file.";

		// validate file that contains timestamps to evaluate
	}
	std::cerr << "usage:\n./btc <db_filename>\n";
	return 1;
}

/*
each line in this file must use the following format: "date | value".
•A valid date will always be in the following format: Year-Month-Day.
•A valid value must be either a float or a positive integer, between 0 and 1000.
- compare to data.csv (should exist)

Your program will use the value in your input file.
Your program should display on the standard output the result of the value multiplied
by the exchange rate according to the date indicated in your database.
If the date used in the input does not exist in your DB then you
must use the closest date contained in your DB. Be careful to use the
lower date and not the upper one.

Example usage:

$> ./btc input.txt
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-03 => 1 = 0.3
2011-01-03 => 1.2 = 0.36
2011-01-09 => 1 = 0.32
Error: not a positive number.
Error: bad input => 2001-42-42
2012-01-11 => 1 = 7.1
Error: too large a number.
$>
*/