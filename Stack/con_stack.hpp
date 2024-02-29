#pragma once
#include "include.h"

namespace Stack
{
	class Stack {
		std::vector<std::string> stack;
		std::map<std::string, std::string> var_stack;
	public:
		void push(std::string data);
		std::string pop();
		std::string pop_index(unsigned int idx); //pop with size()-idx index 
		size_t stacksize();
	};
}
