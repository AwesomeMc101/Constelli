#pragma once

#define OP_CALL 1
#define OP_JMP 2
#define OP_JMPLOC 3 //location that can be jumped to
#define OP_JUMPLOCEND 16 //end jumploc

#define OP_ADD 4
#define OP_SUB 5
#define OP_MUL 6
#define OP_DIV 7
#define OP_MOD 8

#define OP_EQU 9
#define OP_DEQU 18
#define OP_LETA 10 //less than
#define OP_GETA 11 //greater than

#define OP_NEWMEM 12
#define OP_EDITMEM 13
#define OP_MEMREF 14 //var reference [var]

#define OP_STRING 15 //open or close

#define OP_ENDLINE 40

#define OP_OPENPARA 41
#define OP_CLOSEPARA 42
#define OP_OPENVAR 43 //[
#define OP_CLOSEVAR 44 //]

#define OP_UNKNOWN 100

#define OP_INT 56

#define OP_NOINSTRUCTION 101
#define OP_SPLITTER 103

/* keywords */
#define OP_IF 30
#define OP_ENDIF 31


#define OP_PUSHSTACK 400 //return,  call_stack push
#define OP_PUSHCALLSTACK 401

#define NOTECALL 404

#define INST short int



class Instruction_Set
{
public:
	std::vector<std::pair<INST, std::string>> instructions;

};
