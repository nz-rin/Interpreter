#ifndef IR_LEXER_H 
#define IR_LEXER_H

#include <vector>
#include <string>

void lex_intermediate_rep(std::vector<std::string> &token_buffer, const char *ir_file_path);
void tokenize_ir(std::vector<std::string> &token_buffer, std::string IR);

#endif

