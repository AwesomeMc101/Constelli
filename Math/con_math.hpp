#include "include.h"

namespace Arith
{
	bool is_num(std::string num);
	
	std::string add(std::string A, std::string B);
	std::string sub(std::string A, std::string B);
	std::string mul(std::string A, std::string B);
	std::string div(std::string A, std::string B);
	std::string mod(std::string A, std::string B);
	std::string arith(std::string A, std::string B, INST math_call);
}
