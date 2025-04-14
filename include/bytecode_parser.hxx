#ifndef BYTECODE_PARSER
#define BYTECODE_PARSER

#include <vector>
#include <string_view>
#include <unordered_map>

#include "stack.hxx"

void parse_bytecode(Stack &stack, const char *file_name);
void execute_instruction(const char* intruction, const char *parameter);

#endif
