#pragma once
#include <stack>
#include <string>

typedef enum Operator { ADD = '+', SUB = '-', MUL = '*', DIV = '/' } Operator;

class RPN {

public:
	RPN();
	RPN(std::string const& expression);
	void run(Operator op);
	void parse(std::string const& expression);
	void execute(std::string const& expression);
	RPN(RPN const& src);
	~RPN();

	RPN& operator=(RPN const& rhs);

private:
	std::stack<int> _stack;
};