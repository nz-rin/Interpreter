#include "lexer_tools.hxx"
#include <iostream>

bool is_numeric(char c){
	return c >= '0' && c <= '9';
}

bool is_alpha(char c){
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');		
}

bool is_alpha_numeric(char c){
	return is_numeric(c) || is_alpha(c);
}

bool is_numeric(const char *c, int lenght){
	for(int i = 0; i < lenght; i++){
		if( !is_numeric(c[i]) ){
			return false;
		}
	}
	return true;
}

bool is_alpha(const char *c, int lenght){
	for(int i = 0; i < lenght; i++){
		if( !is_alpha(c[i]) ){
			return false;
		}
	}
	return true;
}

bool is_alpha_numeric(const char *c, int lenght){
	return is_numeric(c, lenght) || is_alpha(c, lenght);
}

bool is_white_space(char c){
	return c == ' ' || c == '\t' || c == '\n';
}

bool is_new_line(char c){
	return c == '\n';
}

bool is_null_byte(char c){
	return c == '\0';
}
