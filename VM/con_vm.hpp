#include "include.h"

#include "con_stack.hpp"
#include "con_math.hpp"
#include "con_cfunction.hpp"
#include "con_concat.hpp"
#include "con_compare.hpp"

namespace VM {
	std::string decode_instruction(INST instruction);
	void execute(Instruction_Set* in_St, Stack::Stack* heap);
	
}
