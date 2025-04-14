#include <cstring>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

#include "fileIO.hxx"
#include "stack.hxx"
#include "bytecode_parser.hxx"

#include "IR_parser.hxx"
#include "IR_lexer.hxx"

#include "log.hxx"

int main(int argc, const char *argv[]){
	if(argc < 2){
		err_log_and_exit("[ERROR] Insufficiant Arguments.\n[BUILD FAILURE]");
	}

	init_memory();
	if(strcmp(argv[1], "-c") == 0){
		if(argc <= 3){
			log("[ARG ERROR] No File Path Given");
			return 0;
		}
		std::vector<std::string> ir = std::vector<std::string>();
		lex_intermediate_rep(ir, argv[2]);
		parse_ir_tokens(ir, argv[3]);
		log("[BYTECODE COMPILED SUCCESSFULLY]");
	}
	if(strcmp(argv[1], "-r") == 0){
		if(argc <= 2){
			log("[ARG ERROR] No File Path Given");
			return 0;
		}
		Stack stack= Stack(32);
		parse_bytecode(stack, argv[2]);
		stack.dump();
		log("[RUN SUCCESSFULL]");
	}
	if(strcmp(argv[1], "-t") == 0){
		if(argc <= 2){
			log("[ARG ERROR] No File Path Given");
			return 0;
		}
		std::vector<std::string> ir = std::vector<std::string>();
		ir.reserve(1024);
		lex_intermediate_rep(ir, argv[2]);
		patch_addressing(ir);
		log("[LEX SUCCESSFULL]");
	}
	log_file_states();
	cleanup_memory();
	clean_up_ir_memory();

	return 0;
}
