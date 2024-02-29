#include "con_parse.hpp"



std::vector<std::string> reserved_calls{"print","heap_push","strlen","pow"};
std::vector<char> reserved_tokens{ '(',')','+','-','"','-','/','%',';','\'', '=', '>','<','*' };
std::vector<char> splitter_tokens{ ',' };// these tokens are designed literally just to split things up, like function args. 
std::vector<std::string> reserved_keywords{ "if", "var","goto","jump","entry"};


std::vector<std::string> Parser::parse_line(const char* line)
{
	//tokenize through any reversed tokens

	std::vector<std::string> tokenized_values;
	
	std::cout << "Parsing: " << line << "\n";
	char buffer[2000]{""};
	for (int i = 0; i < strlen(line); i++)
	{
		std::cout << "L: " << line[i] << "\n";
		bool wrote = false;
		for (char c : reserved_tokens)
		{
			if (c == line[i])
			{
				
				if (strlen(buffer)) {
					tokenized_values.push_back(buffer);
					memset(buffer, 0, 2000);
				}
				std::string s(1, line[i]);
				tokenized_values.push_back(s);
				wrote = true;
			}
		}
		for (char c : splitter_tokens)
		{
			if (c == line[i])
			{

				if (strlen(buffer)) {
					tokenized_values.push_back(buffer);
					memset(buffer, 0, 2000);
				}
				std::string s(1, line[i]);
				tokenized_values.push_back(s);
				wrote = true;
			}
		}

		if (!wrote)
		{
			buffer[strlen(buffer)] = line[i];
		}
	}


	//just for debug purposes
	for (int i = 0; i < tokenized_values.size(); i++)
	{
		std::string val = tokenized_values[i];
		std::cout << "Tok Val: " << val << " and strlen " << (val).length() << "\n";
		//std::cout << "Tok[0]: " << val[0] << "\n";
	}

	return tokenized_values;

}

INST Parser::keyword_decode(std::string key)
{
	if (key == "if") { return OP_IF; };
	if (key == "var") { return OP_NEWMEM; }
	if (key == "jump") { return OP_JMP; }
	if (key == "goto") { return OP_JMP; }
	if (key == "entry") { return OP_JMPLOC; }
}

INST Parser::token_decode(char token)
{
	std::cout << "Decode: " << token << "\n";
	if (token == ';') { printf("endline"); return OP_ENDLINE; }
	if (token == '(') { return OP_OPENPARA; }
	if (token == ')') { return OP_CLOSEPARA; }

	if (token == '\'') { return OP_STRING; }
	if (token == '"') { return OP_STRING; }

	if (token == '+') { return OP_ADD; }
	if (token == '-') { return OP_SUB; }
	if (token == '*') { return OP_MUL; }
	if (token == '/') { return OP_DIV; }
	if (token == '%') { return OP_MOD; }

	if (token == '=') { return OP_EQU; } //two of these in a row will trigger more later.
	if (token == '>') { return OP_GETA; }
	if (token == '<') { return OP_LETA; }
}

bool Parser::is_splitter(char splitter)
{
	for (char c : splitter_tokens)
	{
		if (c == splitter) { return true; }
	}
	return false;
}

bool Parser::is_num(std::string num)
{
	std::vector<char> valid_keys = { '0','1','2','3','4','5','6','7','8','9'};
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

Instruction_Set* Parser::new_inst_set(const char* line)
{
	std::vector<std::string> parsed_data = Parser::parse_line(line);

	Instruction_Set* in_St = new Instruction_Set;

	bool creating_string = false;
	std::string string_creation_holster = ""; //used to hold string value.
	/* What is creating_string? Because the parser separates the " from a string, the first " will initiate the 'creating_string'
	variable, which means we will halt production of new instructions until we get another string call.*/

	for (int i = 0; i < parsed_data.size(); i++)
	{
		if (!parsed_data.size()) { continue; }


		if (creating_string)
		{
			if (Parser::token_decode(parsed_data[i][0]) == OP_STRING)
			{
				creating_string = false;
				in_St->instructions.push_back(std::make_pair(OP_STRING, string_creation_holster));
				string_creation_holster.clear();
				continue;
			}
			else
			{
				string_creation_holster.append(parsed_data[i]);
				continue;
			}
		}

		bool done = false;
		for (int call = 0; call < reserved_calls.size(); call++)
		{
			if (parsed_data[i] == reserved_calls[call])
			{
				in_St->instructions.push_back(std::make_pair(OP_CALL, parsed_data[i]));
				done = true;
				continue;
			}
		}
		if (done) { continue; }

		/* keywords */

		for (int key = 0; key < reserved_keywords.size(); key++)
		{
			if (parsed_data[i] == reserved_keywords[key])
			{
				in_St->instructions.push_back(std::make_pair((Parser::keyword_decode(parsed_data[i])), parsed_data[i]));
				done = true;
				continue;
			}
		}
		if (done) { continue; }

		for (int token = 0; token < reserved_tokens.size(); token++)
		{
			if (parsed_data[i][0] == reserved_tokens[token])
			{
				if (Parser::token_decode(parsed_data[i][0]) == OP_STRING)
				{
					creating_string = true;
					done = true;
					continue;
				}
				else {
					in_St->instructions.push_back(std::make_pair((Parser::token_decode(parsed_data[i][0])), parsed_data[i]));

					done = true;
					continue;
				}
			}
		}
		if (done) { continue; }
		if (Parser::is_num(parsed_data[i])) { in_St->instructions.push_back(std::make_pair(OP_INT, parsed_data[i])); done = true; }
		if (done) { continue; }
		if(Parser::is_splitter(parsed_data[i][0])) { done = true; } //was gonna write OP_SPLITTER, realized i dont need to! HAHA
		if (done) { continue; }
		in_St->instructions.push_back(std::make_pair(OP_UNKNOWN, parsed_data[i]));
	}
	return in_St;
}
