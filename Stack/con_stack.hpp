#pragma once
#include "include.h"

namespace Stack
{
	class Stack {
		std::vector<std::string> stack;
		std::map<std::string, std::string> var_stack;
		std::map < std::string, std::vector<Instruction_Set*>> jump_stack;
	public:
		void push(std::string data);
		std::string pop();
		std::string pop_index(unsigned int idx); //pop with size()-idx index 
		void pop_back();
		size_t stacksize();
		void push_var(std::string iden, std::string value);
		std::string index_var(std::string iden);

		void new_jump(std::string iden);
		void push_jump(std::string iden, Instruction_Set* in_St);
		std::vector<Instruction_Set*> return_jump_instructions(std::string iden);

		void dbg_printvars();
	};
}
