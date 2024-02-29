#include "con_concat.hpp"

std::string Concat::concat_str(std::string A, std::string B)
{
	A.append(B);
	std::cout << "\nConcat return: " << A << "\n";
	return A;
}
