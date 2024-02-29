#include "con_vm.hpp"

void VM::execute(Instruction_Set* in_St, Stack::Stack* heap)
{
	static bool is_jumploc = false;

	static std::string jumploc_iden = "";

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

		
		if (instr == OP_JUMPLOCEND)
		{
			is_jumploc = false;
		}
		else if (is_jumploc)
		{
			//std::cout << "ISJUMPLOC";
			heap->push_jump(jumploc_iden, in_St);
			if (in_St->instructions.size() >= i)
			{
				if (in_St->instructions[i + 1].first == OP_JUMPLOCEND)
				{
					is_jumploc = false;
				}
			}
			return;
		}

		if (instr == OP_CALL)
		{
			//call cfunction w/ stack
			stack->push(CFunction::do_cfunction(data, stack->pop(), stack->pop_index(1))); //data is like "print"
		}
		else if (instr == OP_JMP)
		{
			std::string iden = stack->pop();
			std::vector<Instruction_Set*> in_St_T = heap->return_jump_instructions(iden);

			stack->pop_back(); //get rid of identifier from stack

			for (int i = 0; i < in_St_T.size(); i++)
			{
				std::reverse(in_St_T[i]->instructions.begin(), in_St_T[i]->instructions.end());
				for (int x = 0; x < in_St_T[i]->instructions.size(); x++)
				{
					in_St->instructions.push_back(in_St_T[i]->instructions[x]);
				}
				//in_St->instructions
			}
		}
		else if (instr == OP_JMPLOC)
		{
			//new jump
			heap->new_jump(stack->pop());
			jumploc_iden = stack->pop();
			is_jumploc = true;
		}

		else if (instr == OP_NEWMEM)
		{
			if (stack->stacksize() >= 1)
			{
				//std::string dbgstr = stack->pop();
				heap->push_var(stack->pop(), stack->pop_index(1));

				//std::cout << "new var " << " valued at " << heap->index_var(dbgstr) << "\n";
				heap->dbg_printvars();
			}
			else
			{
				//printf("var cannot be called");
			}
		}
		else if (instr == OP_INT)
		{
			stack->push(data);
			bool do_math = false;
			switch (old_instr) //check if we need to do a math call
			{
			case OP_ADD: do_math = true; break;
			case OP_SUB: do_math = true; break;
			case OP_MUL: do_math = true; break;
			case OP_DIV: do_math = true; break;
			case OP_MOD: do_math = true; break;
			}
			//if (!do_math) //no math, push int to stack
			//{
			//	stack->push(data);
			//	continue;
			//}
			// math
			if (do_math) {
				stack->push(Arith::arith(stack->pop(), stack->pop_index(1), old_instr)); //push arith val to stack
			}
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
		else if (instr == OP_MEMREF)
		{
			stack->push(heap->index_var(data));

		//	std::cout << "memref: " << heap->index_var(data) << "\n";

			bool do_math = false;
			switch (old_instr) //check if we need to do a math call
			{
			case OP_ADD: do_math = true; break;
			case OP_SUB: do_math = true; break;
			case OP_MUL: do_math = true; break;
			case OP_DIV: do_math = true; break;
			case OP_MOD: do_math = true; break;
			}
			if (do_math) {
				if (!Arith::is_num(stack->pop()))
				{

					stack->push(Concat::concat_str(stack->pop(), stack->pop_index(1)));
				}
				else
				{

					stack->push(Arith::arith(stack->pop(), stack->pop_index(1), old_instr));
				}
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
				/*if (old_instr == OP_CLOSEVAR)
				{
					stack->push(heap->index_var(data));
				}*/
				if (old_instr == OP_STRING) //meaning that on the old instruction, it had passed a value stating string was opened. may need
				//to verify that it wasnt a close string
				{
					stack->push(data);
				}
				else if (old_instr == OP_EQU)
				{
					stack->push(data); //var name
				}
			}
		}

	}
}
