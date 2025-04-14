
#include "log.hxx"
#include "fileIO.hxx"

#include "IR_lexer.hxx"

void lex_intermediate_rep(std::vector<std::string> &token_buffer, const char *ir_file_path){
	FileID ir_source = open_file(ir_file_path);
	if(!token_buffer.empty()){token_buffer.clear();}
	tokenize_ir(token_buffer, read_all_from_file(ir_source));
	close_file(ir_source);
}

void tokenize_ir(std::vector<std::string> &token_buffer, std::string IR_source){
	
	std::string token = "";	
	log("[LEXING IR]");
	log("  chars to lex:", IR_source.size());
	d_log("  IR to Lex:");
	d_log(IR_source);

	for(size_t i = 0; i < IR_source.size() ; i++){
		switch(IR_source[i]){
			case '[':{if(IR_source[i+1] == ']') continue;} break;
			case ']': continue;
			case '@': continue;
			case '-': continue;
			case '>': continue;
			case ' ': continue;
			case '\t': continue;
		};
		if(IR_source[i] == '\"'){
			do{
				if(i >= IR_source.size()){
					err_log_and_exit("[LEXER ERROR] Incomplete String : ", token);
				}
				token += (IR_source[i]);
			}while(IR_source[++i] != '\"');
			token += (IR_source[i]);
			token_buffer.push_back(token);
			token.clear();
			if(IR_source[i+1] == '\n'){i++;}
			continue;
		}		
		if(IR_source[i+1] == ']'){
			token += IR_source[i];
			token += IR_source[++i];
			token_buffer.push_back(token);
			token.clear();
			i++;
			continue;
		}
		if(IR_source[i+1] == '\n'){
			token += IR_source[i];
			token_buffer.push_back(token);
			token.clear();
			i++;
			continue;
		}
		if(IR_source[i+1] == '\0'){
			token += IR_source[i];
			token_buffer.push_back(token);
			token.clear();
			break;
		}
		if(IR_source[i+1] == ' '){
			token += IR_source[i];
			token_buffer.push_back(token);
			token.clear();
			continue;
		}
		
		token += IR_source[i];

	};
	//O(n^2) worse case algorithim
	for(size_t i = 0; i < token_buffer.size(); i++){
		if(token_buffer[i].size() == 1 && token_buffer[i][0] == '\n'){
			token_buffer.erase(token_buffer.begin() + i--);
		}
	}
	
	/* Faster O(n) algorithim
	token_buffer.erase(
		std::remove_if( token_buffer.begin(), [](const std::string &o)->{ o[0] != '\n'}),
		token_buffer.end();
			);
	*/
	#ifdef DLOG
	for(std::string t : token_buffer){
		std::cout << "TOKEN: [" << t << "]" << " Token Size" << t.size() <<"\n";
	}
	std::cout << "\n";
	#endif

	log("    NUMBER OF TOKENS OBTAINED", token_buffer.size());
	log("[IR LEXING SUCCESSFULL]");
}
