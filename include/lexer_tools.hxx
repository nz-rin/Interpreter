#ifndef LEXER_TOOLS_H
#define LEXER_TOOLS_H

bool is_numeric(char c);
bool is_alpha(char c);
bool is_alpha_numeric(char c);
bool is_numeric(const char *c, int lenght);
bool is_alpha(const char *c, int lenght);
bool is_alpha_numeric(const char *c, int lenght);
bool is_white_space(char c);
bool is_new_line(char c);
bool is_null_byte(char c);

#endif
