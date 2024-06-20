#pragma once
#include <stack>
#include <string>

class RPN {

public:
	RPN();
	RPN(std::string expression);
	RPN(RPN const& src);
	~RPN();

	RPN& operator=(RPN const& rhs);

private:
	std::stack<int> _stack;
};