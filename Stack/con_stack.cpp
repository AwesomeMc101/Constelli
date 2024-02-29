#include "con_stack.hpp"

std::string Stack::Stack::pop()
{
	return Stack::stack[Stack::stack.size() - 1]; 
}

std::string Stack::Stack::pop_index(unsigned int idx) //top value is size()-1, - idx from this
{
	if (idx >= Stack::stack.size())
	{
		std::cout << "\n\nERROR: ATTEMPT AT INDEXING STACK OUT OF RANGE.\n\n";
		return "0";
	}
	return Stack::stack[Stack::stack.size() - 1 - idx]; //5 - 1 - 5
}

void Stack::Stack::push(std::string data)
{
	std::cout << "pushstack: " << data << "\n";
	Stack::stack.push_back(data);
}

size_t Stack::Stack::stacksize()
{
	return Stack::stack.size();
}
