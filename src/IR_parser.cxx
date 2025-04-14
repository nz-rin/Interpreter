
#include <cstring>
#include <vector>

#include "fileIO.hxx"
#include "data.hxx"
#include "log.hxx"

#include "IR_parser.hxx"

char *number_buf = new char[sizes::LONG];

std::unordered_map<std::string, std::pair<size_t, size_t>> map_of_addresses = std::unordered_map<std::string, std::pair<size_t, size_t>>();

void parse_ir_tokens(std::vector<std::string> &Tokens, const char *file_name){
	patch_addressing(Tokens);

	FileID bc = create_file(file_name);


	log("[CREATING BYTECODE]");
	char *c_token;
	log("[TOKENS]", Tokens.size());

	for(size_t i = 0; i < Tokens.size(); i++){
		c_token = Tokens[i].data();	

		if(is_instruction(c_token)){
			unsigned char opcode = instruction::set.at(c_token);

			d_log("[INST]", c_token);
			write_instruction(bc, opcode);
			if(is_arithmetic_inst(opcode)){
				d_log("[MATH OP]");
				if(++i == Tokens.size()){
					err_log_and_exit("  [ERROR] Insufficient Arguments");
				}
				write_bytecode_byte_size_quantifier(bc, get_ir_byte_size_from_quantifier(Tokens[i].data()));
				continue;
			}
			if(is_memory_inst(opcode)){
				d_log("[MEMORY OP]");
				if(is_jump_type(opcode)){
					d_log("  [INSTRUCTION] JMP");
					if(++i >= Tokens.size()){
						err_log_and_exit("[ERROR] No address to jump to");
					}
					write_numeric_byte_value(bc, sizes::LONG, Tokens[i].data());	
					continue;
				}
				if(opcode == instruction::CMP){
					d_log("  [INSTRUCTION] CMP");
					if(++i >= Tokens.size()){
						err_log_and_exit("[ERROR] No Byte Size Given to compare");
					}
					size_t memory_size = get_ir_byte_size_from_quantifier(Tokens[i].data());
					write_bytecode_byte_size_quantifier(bc, memory_size);
					continue;
				}

				if(opcode == instruction::LOAD){
					number_buf[0] = registers.at(Tokens[++i]);
					number_buf[1] = registers.at(Tokens[++i]);
					write_to_file(bc, number_buf , 2);
					continue;
				}

				//ADD HEAP EVENTUALY
				#ifdef DLOG
					if(opcode == instruction::PUSH){
						d_log("  [INSTRUCTION] PUSH");
					}
					if(opcode == instruction::POP){
						d_log("  [INSTRUCTION] POP");
					}
				#endif
				//PUSH POP OPCODE
				if(opcode == instruction::POP || opcode == instruction::PUSH){
					if(i+2 >= Tokens.size()){
						err_log_and_exit("  [ERROR] insufficient arguments for opcode", c_token);
					}
					size_t memory_size = get_ir_byte_size_from_quantifier(Tokens[++i].data());
					write_bytecode_byte_size_quantifier(bc, memory_size);

					if(Tokens[++i][0] == '$'){
						write_numeric_byte_value(bc, memory_size, &Tokens[i].data()[1]);
					}else{
						write_numeric_byte_value(bc, memory_size, Tokens[i].data());
					}

					continue;
				}
				
				err_log_and_exit("[OPCODE ERROR] Unimplemented Memory Opcode", opcode);


			}
		}
	};
	
}

void patch_addressing(std::vector<std::string> &Tokens){
	log("[PATCHING ADDRESSES]");
	log("  Token Size:", Tokens.size());

	size_t bytes = 0;
	for(size_t i = 0; i < Tokens.size(); i++){
		log("[TOKEN INFO]\n  TOKEN :", Tokens[i], "\n  BYTE", bytes, "\n");
		if(is_ir_byte_size_quantifier(Tokens[i])){
			bytes +=1;
			bytes += get_ir_byte_size_from_quantifier(Tokens[i]);
			i++;
			continue;
		}
		if(is_instruction(Tokens[i])){
			bytes += 1;
			continue;
		}

		if(Tokens[i][0] == '['){
			if(is_instruction(Tokens[i-1]) && is_jump_type(instruction::set.at(Tokens[i-1]) )){
				map_of_addresses[Tokens[i]].first = bytes;	
				map_of_addresses[Tokens[i]].second += 1;	
				bytes += sizes::LONG;
				continue;
			}
			map_of_addresses[Tokens[i]].first = bytes-1;	
			map_of_addresses[Tokens[i]].second += 1;	
		}
	}

//#ifdef DLOG
	for( std::pair<std::string, std::pair<size_t, size_t>> _: map_of_addresses){
		bytes -= sizes::LONG;
		log(_.first, " | ADR: ", _.second.first, " Ocurances: ", _.second.second);
	}
	log("MAP SIZE", map_of_addresses.size());
//#endif

	//log("BYTES", bytes);

	for(size_t i = 0; i < Tokens.size(); i++){
		if(map_of_addresses.find(Tokens[i]) == map_of_addresses.end()){
			continue;
		}
		if(map_of_addresses[Tokens[i]].second == 1){
			Tokens.erase(Tokens.cbegin() + i);
			continue;
		}

		map_of_addresses[Tokens[i]].second -= 1;
		Tokens[i] = std::to_string(map_of_addresses[Tokens[i]].first);
	}

}

bool is_arithmetic_inst(unsigned char opcode){	
	return (math_table.find(opcode) == math_table.end()? false:true);
}
bool is_memory_inst(unsigned char opcode){	
	return (memory_table.find(opcode) == memory_table.end()? false:true);
}
bool is_jump_type(unsigned char opcode){
	return opcode == instruction::JMP || opcode == instruction::JMP_EQ || opcode == instruction::JMP_NEQ;
}

void get_raw_value_from_string(size_t byte_size, std::string_view numeric_value_as_string){
	ssize_t raw_bytes = std::atoll(&numeric_value_as_string[0]);	
	memcpy(number_buf, &raw_bytes, byte_size);

}

void write_bytecode_byte_size_quantifier(FileID bytecode_file_path_and_name, size_t byte_size){
	write_to_file(bytecode_file_path_and_name, (char *)&byte_size, 1);
}

void write_numeric_byte_value(FileID bytcode_file_path_and_name, size_t byte_size, std::string_view byte_data){
	if(byte_data[0]=='$'){
		get_raw_value_from_string(byte_size, &byte_data[1]);
		//log("[byte data[1]]", byte_data[1]);
	}else{
		get_raw_value_from_string(byte_size, &byte_data[0]);
	}
	write_to_file(bytcode_file_path_and_name, number_buf, byte_size);
}

void write_instruction(FileID bytecode_file, unsigned char opcode){
	write_to_file(bytecode_file, (char*)&opcode, 1);
}

bool is_instruction(std::string_view bytecode){
	return (instruction::set.find(bytecode) == instruction::set.end()) ? false: true;	
}

bool is_ir_byte_size_quantifier(std::string_view possible_size_quantifier){
	return (sizes::quantifiers.find(possible_size_quantifier) == sizes::quantifiers.end() )? false: true;
}

size_t get_ir_byte_size_from_quantifier(std::string_view ir_byte_size_quantifier){
	return sizes::quantifiers.at(ir_byte_size_quantifier);
}

void clean_up_ir_memory(){
	delete[] number_buf;
}
