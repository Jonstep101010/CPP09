#include "RPN.hpp"
#include <cstdlib>
#include <iostream>
#include <ostream>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RPN::RPN() {}

static bool pairs_match_operators(std::string const& expression) {
	size_t operators = 0;
	size_t numbers   = 0;
	for (size_t i = 0; i < expression.size(); i++) {
		if (isdigit(expression[i])) {
			numbers++;
		}
		if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*'
			|| expression[i] == '/') {
			operators++;
		}
	}
	return operators == numbers - 1;
}

/**
 * @brief if the expression is invalid, print an error message and exit
 * @details can only contain numbers and operators, spaces are used to separate them
 * \details an operator can be +, -, *, or /
 */
static void validate(std::string const& expression) {
	// expression cannot be empty or a single character
	if (expression.empty() || expression.size() == 1) {
		std::cerr << "Error\n";
		exit(1);
	}
	// expression starts with a number
	if (!isdigit(expression.at(0))) {
		std::cerr << "Error\n";
		exit(1);
	}
	// expression ends with an operator
	if (expression[expression.length() - 1] != '+'
		&& expression[expression.length() - 1] != '-'
		&& expression[expression.length() - 1] != '*'
		&& expression[expression.length() - 1] != '/') {
		std::cerr << "Error\n";
		exit(1);
	}
	// guarantee that only numbers, operators, and spaces are present
	if (expression.find_first_not_of("0123456789+-*/ ") != std::string::npos) {
		std::cerr << "Error\n";
		exit(1);
	}
	// a number cannot be greater than 9 (single digit)
	for (size_t i = 0; i < expression.size(); i++) {
		if (isdigit(expression[i]) && expression[i + 1] != ' ') {
			std::cerr << "Error\n";
			exit(1);
		}
		if (isspace(expression[i]) && isspace(expression[i + 1])) {
			std::cerr << "Error\n";
			exit(1);
		}
	}
	// make sure there are as many operators as there are pairs of numbers
	if (!pairs_match_operators(expression)) {
		std::cerr << "Error\n";
		exit(1);
	}

	std::cout << "expression: " << expression << " passed validation" << std::endl;
}

/**
 * @brief execute the RPN expression
 * @note requires _stack to be filled with the parsed expression
 */
void RPN::run() {
	// scan until encountering an operator
	// pop the last two numbers from the stack
	// apply the operator
	// push the result back onto the stack
}

/**
 * @brief Construct a new RPN::RPN object using
 * 
 * @param[in] expression 
 */
RPN::RPN(std::string const& expression) {
	validate(expression);
	// parse
	RPN::parse(expression);
	// execute
}

RPN::RPN(const RPN& src)
	: _stack(src._stack) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

RPN::~RPN() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

RPN& RPN::operator=(RPN const& rhs) {
	//if ( this != &rhs )
	//{
	//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */