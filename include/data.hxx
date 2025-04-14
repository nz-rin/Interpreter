#ifndef DATA_H
#define DATA_H
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#define Opcode const static unsigned char
#define uchar const static unsigned char


namespace dType{
	uchar BIT8  = 1;
	uchar BIT16 = 2;
	uchar BIT32 = 4;
	uchar BIT64 = 8;
};

namespace sizes{
	const size_t BYTE = sizeof(char);
	const size_t SHORT = sizeof(short);
	const size_t INT = sizeof(int);
	const size_t LONG = sizeof(size_t);

	const static std::unordered_map<std::string_view, size_t> quantifiers = {
		{"i8" , BYTE},
		{"i16", SHORT},
		{"i32", INT},
		{"i64", LONG},
		{"f32", INT},
		{"f64", LONG},
	};
};

namespace instruction{
	const static char REG_SP = 0;
	const static char REG_BP = 1;

	const static char EQ = 0; 
	const static char LESS = 1; 
	const static char GREATER = 2; 

	Opcode UNEXPECTED_OPCODE = -1;
	
	Opcode PUSH = 1;
	Opcode POP = 2;
	Opcode LOAD= 3;
	
	Opcode ADD = 4;
	Opcode SUB = 5;

	Opcode FADD = 6;
	Opcode FSUB = 7;

	Opcode CMP = 8;

	Opcode JMP = 9;
	Opcode JMP_EQ = 10;
	Opcode JMP_NEQ = 11;
	
	Opcode OUT = 12;
	Opcode IN = 13;

	const static std::unordered_map<std::string_view, unsigned char> set{
		{"Push", PUSH}, 
		{"Pop", POP}, 
		{"Load", LOAD}, 

		{"Add", ADD}, 
		{"Sub", SUB}, 

		{"FAdd", FADD}, 
		{"FSub", FSUB}, 

		{"Cmp", CMP}, 

		{"Jump_NC", JMP}, 
		{"Jump_EQ", JMP_EQ}, 
		{"Jump_NEQ", JMP_NEQ}, 

		{"Out", OUT},
		{"In", IN}

	};
}

const static std::unordered_map<std::string_view, unsigned char> registers  = {
	{"RSP", 0},
	{"RBP", 1}
};

const static std::unordered_set<unsigned char> math_table = {
	instruction::ADD,
	instruction::SUB,
	instruction::FADD,
	instruction::FSUB
};

const static std::unordered_set<unsigned char> memory_table = {
	instruction::PUSH,
	instruction::POP,
	instruction::CMP,
	instruction::JMP,
	   
	instruction::JMP_EQ,
	instruction::JMP_NEQ,

	instruction::LOAD
};

#endif
