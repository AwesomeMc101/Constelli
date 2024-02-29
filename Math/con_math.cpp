#include "con_math.hpp"


bool Arith::is_num(std::string num)
{
	std::vector<char> valid_keys = { '0','1','2','3','4','5','6','7','8','9' };
	char period = '.'; //lmao
	char space = ' ';
	int number_vals = 0;
	for (int i = 0; i < num.size(); i++)
	{
		char comp = num[i];
		bool is_digit = false;
		for (char c : valid_keys)
		{
			if (comp == c) { is_digit = true; number_vals++; continue; }
		}
		if (!is_digit)
		{
			//	if (i == 0) { return false; } //no choice, c
			if ((comp != period) && (comp != space)) { return false; }
		}
	}
	if (number_vals == 0) { return false; }
	return true;
}

std::string Arith::add(std::string A, std::string B)
{
	double Ai = std::stod(A);
	double Bi = std::stod(B);

	return std::to_string(Ai + Bi);
}


std::string Arith::sub(std::string A, std::string B)
{
	double Ai = std::stod(A);
	double Bi = std::stod(B);

	return std::to_string(Ai - Bi);
}
std::string Arith::mul(std::string A, std::string B)
{
	double Ai = std::stod(A);
	double Bi = std::stod(B);

	return std::to_string(Ai * Bi);
}
std::string Arith::div(std::string A, std::string B)
{
	double Ai = std::stod(A);
	double Bi = std::stod(B);

	return std::to_string(Ai / Bi);
}
std::string Arith::mod(std::string A, std::string B)
{
	int Ai = std::stoi(A);
	int Bi = std::stoi(B);

	return std::to_string(Ai % Bi);
}

std::string Arith::arith(std::string A, std::string B, INST math_call)
{
	if (!Arith::is_num(A) || !Arith::is_num(B))
	{
		return A;
	}
	
	switch (math_call)
	{
	case OP_ADD:
		return Arith::add(A, B);
		break;
	case OP_SUB:
		return Arith::sub(A, B);
	case OP_MUL:
		return Arith::mul(A, B);
	case OP_DIV:
		return Arith::div(A, B);
	case OP_MOD:
		return Arith::mod(A, B);
	}
	return A;
}
