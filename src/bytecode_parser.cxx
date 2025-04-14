#include <cstring>

#include "fileIO.hxx"
#include "lexer.hxx"
#include "log.hxx"
#include "stack.hxx"
#include "utils.hxx"
#include "data.hxx"

#include "bytecode_parser.hxx"

Stack *m_stack;

struct reg{
	char crf = 0;

	short srf = 0;

	int irf = 0;

	float f32f = 0;

	double f64f = 0;

	ssize_t lrf = 0;

	size_t RBP = 0;
	size_t RSP = 6;
};
reg reg;

void add(unsigned char byte_size, struct reg &reg, char *lhs, char *rhs){
	switch(byte_size){
		case dType::BIT8:
			reg.crf = (char)*lhs+ (char)*rhs;
			m_stack->push(byte_size, &reg.crf);
			break;
		case dType::BIT16:
			reg.srf = pcast::Short(lhs) + pcast::Short(rhs);
			m_stack->push(byte_size, adrcast::Char(&reg.srf) );
			break;
		case dType::BIT32:
			break;
		case dType::BIT64:
			break;
		default:
			err_log("Add error");
	}
}
void sub(unsigned char byte_size, struct reg &reg, char *lhs, char *rhs){
	switch(byte_size){
		case dType::BIT8:
		{
			reg.crf = *lhs - *rhs;
			m_stack->push(byte_size, &reg.crf);
			break;
		};
		case dType::BIT16:
		{
			reg.srf = pcast::Short(lhs) - pcast::Short(rhs);
			m_stack->push(byte_size, adrcast::Char(&reg.srf) );
			break;
		};
		case dType::BIT32:
			break;
		case dType::BIT64:
			break;
		default:
			err_log("Add error");
	}
}
void fadd(unsigned char byte_size, struct reg &reg, char *lhs, char *rhs){
	switch(byte_size){
		case dType::BIT32:
		{
			reg.f32f = pcast::Float(lhs)+ pcast::Float(rhs);
			m_stack->push(byte_size, adrcast::Char(&reg.f32f) );
			break;
		}
		case dType::BIT64:
			reg.f64f = pcast::Double(lhs) - pcast::Double(rhs);
			m_stack->push(byte_size, adrcast::Char(&reg.f64f));
			break;
		default:
			err_log("Add error");
	}
}
void fsub(unsigned char byte_size, struct reg &reg, char *lhs, char *rhs){
	switch(byte_size){
		case dType::BIT32:
			reg.f32f = pcast::Float(lhs) - pcast::Float(rhs);
			m_stack->push(byte_size, adrcast::Char(&reg.f32f) );
			break;
		case dType::BIT64:
			reg.f64f = pcast::Double(lhs) - pcast::Double(rhs);
			m_stack->push(byte_size, adrcast::Char(&reg.f64f));
			break;
		default:
			err_log("Add error");
	}
}

size_t *get_reg(unsigned char reg_id){
	switch(reg_id){
		case instruction::REG_BP:
			return &reg.RBP;
		case instruction::REG_SP:
			return &reg.RSP;
		default:
			err_log_and_exit("[REGISTER ERROR] Unkown Register", (int)reg_id);
			return new size_t;
	}
}
	
void load(unsigned char reg_to_load_from, unsigned char reg_to_load_into){

	*get_reg(reg_to_load_into) = *get_reg(reg_to_load_from);
	
}

void compare(unsigned char byte_size, void *v1, void *v2){
	int memcmp_res = memcmp(v1, v2, byte_size);
	if(memcmp_res == 0){
		m_stack->push(&instruction::EQ);
	}else if(memcmp_res < 0){
		m_stack->push(&instruction::LESS);
	}else{
		m_stack->push(&instruction::GREATER);
	}
}

void parse_bytecode(Stack &stack, const char *file_name){
	m_stack = &stack;

	FileID bytecode_file = open_file(file_name);
	std::string bytecode = read_all_from_file(bytecode_file);
	
	size_t read_size = bytecode.size();
	log("[PARSING BYTCODE]");
	log("  READING", read_size, "BYTES\n");
	
	using namespace instruction;

	for(size_t IP = 0; IP < read_size; IP++){
		d_log("[IP]", IP);
		d_log("[BYTECODE]", (int) bytecode[IP]);
		switch(bytecode[IP]){
			case PUSH:
			{
				d_log("  [INSTRUCTION] PUSH");
				stack.push(bytecode[IP+1], &bytecode[IP+2]);
				++IP;
				IP += bytecode[IP];
				break;
			};
			case POP:
			{
				err_log("[UNIMPLEMENTED] Pop");
				break;	
			};
			case LOAD:
			{
				d_log("LOAD");
				d_log("BEFORE");
				d_log("  RSP", reg.RSP);
				d_log("  jRBP", reg.RBP);
				unsigned char reg_to_load_from = bytecode[++IP]; 
				unsigned char reg_to_load_to = bytecode[++IP];
				load(reg_to_load_from, reg_to_load_to);
				d_log("");
				d_log("AFTER");
				d_log("  RSP", reg.RSP);
				d_log("  RBP", reg.RBP);
				break;
			};
			case ADD:
			{
				d_log("  [INSTRUCTION] ADD");
				unsigned char byte_size = bytecode[++IP];
				d_log("    [BYTESIZE]", (int) byte_size);
				add(byte_size, reg, stack.pop(byte_size), stack.pop(byte_size));
				break;
			};
			case SUB:
			{
				d_log("  [INSTRUCTION] SUB");
				unsigned char byte_size = bytecode[++IP];
				d_log("    [BYTESIZE]", (int) byte_size);
				char *lhs = stack.pop(byte_size);
				char *rhs = stack.pop(byte_size);
				sub(byte_size, reg, lhs, rhs);
				break;
				
			};
			case FADD:
			{
				unsigned char byte_size = bytecode[++IP];
				fadd(byte_size, reg, stack.pop(byte_size), stack.pop(byte_size));
				break;

			};
			case FSUB:
			{
				unsigned char byte_size = bytecode[++IP];
				char *lhs = stack.pop(byte_size);
				char *rhs = stack.pop(byte_size);
				fsub(byte_size, reg, lhs, rhs);
				break;

			};
			case CMP:
			{
				unsigned char byte_size = bytecode[++IP];
				d_log("  [INSTRUCTION] CMP\n    [BYTE SIZE]", (int)byte_size);
				char *v1 = stack.pop(byte_size);
				char *v2 = stack.pop(byte_size);
				compare(byte_size, v1, v2);		
				break;

			};
			case JMP:
			{
				IP = pcast::Long(&bytecode[++IP]);
				break;
			};
			case JMP_EQ:
			{
				if(*stack.pop() == 0){
					IP=pcast::Long(&bytecode[++IP]);
					break;
				}
				IP += sizes::LONG;
				break;
			};
			case JMP_NEQ:
			{
				if(*stack.pop() != 0){
					IP=pcast::Long(&bytecode[++IP]);
					break;
				}
				IP += sizes::LONG;
				break;
			};
			default:
				err_log("[UNKOWN BYTECODE]", (int)bytecode[IP], "At Instruction Pointer", IP);
				IP++;
		}
	}
}
