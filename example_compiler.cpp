#include <iostream>

#include "con_parse.hpp"
#include "con_vm.hpp"

int main()
{

std::vector<std::string> lines = { 
  "entry 'first'" ,
		"print();",
		"leave;",

		"var nv = 5 + 5;" ,
		"goto 'first' [nv];",

	};
	Stack::Stack* heap = new Stack::Stack;
	for (std::string line : lines)
	{
	Instruction_Set* in_St = Parser::new_inst_set(line.c_str());
	VM::execute(in_St, heap);
	}
}
