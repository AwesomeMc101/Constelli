#include "con_vm.hpp"

void VM::execute(Instruction_Set* in_St, Stack::Stack* heap)
{
	std::reverse(in_St->instructions.begin(), in_St->instructions.end()); //reverse interpretation

	Stack::Stack* stack = new Stack::Stack;
	for (int i = 0; i < in_St->instructions.size(); i++)
	{
		INST instr = in_St->instructions[i].first;
		INST old_instr = OP_NOINSTRUCTION;
		if (i > 0) { old_instr = in_St->instructions[i - 1].first; } //old data is popped to the stack...
		std::string data = in_St->instructions[i].second;

		

		//decided to do if..else instead of switch as switches seem just too
		//wild for this LOL

		if (instr == OP_CALL)
		{
			//call cfunction w/ stack
			std::cout << "opcall\n";
			stack->push(CFunction::do_cfunction(data, stack->pop(), stack->pop_index(1))); //data is like "print"
		}
		else if (instr == OP_INT)
		{
			bool do_math = false;
			switch (old_instr) //check if we need to do a math call
			{
			case OP_ADD: do_math = true; break;
			case OP_SUB: do_math = true; break;
			case OP_MUL: do_math = true; break;
			case OP_DIV: do_math = true; break;
			case OP_MOD: do_math = true; break;
			}
			if (!do_math) //no math, push int to stack
			{
				stack->push(data);
				continue;
			}
			// math
			stack->push(Arith::arith(data, stack->pop(), old_instr)); //push arith val to stack
		}

		else if ((instr == OP_ADD) || (instr == OP_SUB) || (instr == OP_MUL) || (instr == OP_DIV) || (instr == OP_MOD))
		{
			if (!stack->stacksize()) //verify we have a stack value to index
			{
				if (!heap->stacksize())
				{
					stack->push("0");
				}
				else
				{
					stack->push(heap->pop());
				}
			}
		}
		else if (instr == OP_STRING)
		{
			stack->push(data);
			if (old_instr == OP_ADD)
			{
				stack->push(Concat::concat_str(stack->pop(), stack->pop_index(1))); 
			}
		}

		else if (instr == OP_UNKNOWN) //this is just a backup for a lot of the time now... most "fixes" in here are now fixed in parser 
		{
			//chekc for empty whitespace, if its just OP_UNKNOWN with a bunch of spaces we can disregard and set next loops old_instr to our old_instr (i-2 for them)
			bool not_empty = false;
			for (int i = 0; i < data.size(); i++)
			{
				if (data[i] != ' ') { not_empty = true; }
			}
			if (!not_empty)
			{
				in_St->instructions[i].first = old_instr;
				/* Why are we doing this? This code's data is a bunch of spaces. We need to be able to access the REAL old instruction,
				example:
				'Hello' + 'World'
				.......^This whitespace could cause us problems.*/
			}
			else
			{
				if (old_instr == OP_STRING) //meaning that on the old instruction, it had passed a value stating string was opened. may need
				//to verify that it wasnt a close string
				{
					stack->push(data);
				}
			}
		}

	}
}
