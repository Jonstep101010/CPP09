#include "BitcoinExchange.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) { (void)src; }

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

BitcoinExchange::~BitcoinExchange() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

BitcoinExchange& BitcoinExchange::operator=(BitcoinExchange const& rhs) {
	//if ( this != &rhs )
	//{
	//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

// std::ostream& operator<<(std::ostream& o, BitcoinExchange const& i) {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }

/*
** --------------------------------- METHODS ----------------------------------
*/

// @todo
static bool check_date(std::string linedate) {
	(void)linedate;
	return true;
}

// @todo
static bool check_value(std::string lineval) {
	(void)lineval;
	return true;
}

// @todo
bool validate_input(std::string db_filename) {
	(void)db_filename;
	check_date(NULL);
	check_value(NULL);
	return true;
}

bool BitcoinExchange::init(std::string db_filename) {
	// parse internal data.csv...
	return validate_input(db_filename);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */