#ifndef IR_PARSER_H
#define IR_PARSER_H

#include "fileIO.hxx"

#include <string>
#include <vector>

void patch_addressing(std::vector<std::string> &Tokens);
void parse_ir_tokens(std::vector<std::string> &Tokens, const char *output_file);

bool is_instruction(std::string_view bytecode);
bool is_ir_byte_size_quantifier(std::string_view possible_size_quantifier);

void write_instruction(FileID bytecode_file, unsigned char opcode);
void write_numeric_byte_value(FileID bytecode_file_path_and_name, size_t byte_size, std::string_view numeric_byte_data);
void write_bytecode_byte_size_quantifier(FileID bytcode_file_path_and_name, size_t byte_size);

size_t get_ir_byte_size_from_quantifier(std::string_view ir_quantifier);

bool is_arithmetic_inst(unsigned char opcode);	
bool is_memory_inst(unsigned char opcode);	
bool is_jump_type(unsigned char opcode);

void clean_up_ir_memory();

#endif
