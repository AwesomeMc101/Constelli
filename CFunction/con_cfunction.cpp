#include "con_cfunction.hpp"

void CFunction::print(std::string value)
{
	std::cout << "\nCONSTELLI || PRINTING: " << value << "\n";
}

int CFunction::strlen(std::string value)
{
	return (value.length());
}

double CFunction::con_pow(std::string A, std::string B)
{
	//std::cout << "POW " << A << "^" << B << "\n";
	return (pow(std::stod(A), std::stod(B)));
}
std::string CFunction::bool_parse(std::string boolean)
{
	if (boolean == "1") { return "true"; }
	return "false";
}

std::string CFunction::get_input()
{
	//printf("calling getinput");
	std::cout << std::endl;
	std::string value = "";
	putchar('>');
	std::getline(std::cin, value);
	return value;
}

std::string CFunction::do_cfunction(std::string caller, std::string arg1, std::string arg2)
{
	if (caller == "print") { CFunction::print(arg1); return "1"; }
	if (caller == "strlen") { return std::to_string(CFunction::strlen(arg1)); }
	if (caller == "pow") { return std::to_string(CFunction::con_pow(arg1, arg2)); }
	if (caller == "bool_parse") { return CFunction::bool_parse(arg1); }
	if (caller == "get_input") { return CFunction::get_input(); }
	//if(caller == ""){}
}
