#include "include.h"

//{'(',')','+','-','"','-','/','%',';'};

namespace Parser
{

	std::vector<std::string> parse_line(const char* line);

	INST keyword_decode(std::string key);
	INST token_decode(char token);
	
	bool is_num(std::string num);
	bool is_splitter(char splitter);
	
	Instruction_Set* new_inst_set(const char* line);

}
