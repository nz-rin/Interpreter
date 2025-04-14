#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#include "log.hxx"
#include "fileIO.hxx"

#include "lexer_tools.hxx"

#include "lexer.hxx"


void _grow_buffer(char *&buffer, ssize_t &buffer_size){
	char *new_buffer = new char[buffer_size+buffer_size];
	for(ssize_t i = 0; i < buffer_size; i++){
		new_buffer[i] = buffer[i];
	}
	delete[] buffer;
	buffer = new_buffer;
	buffer_size += buffer_size;
}
void _grow_buffer(char *&buffer, ssize_t &buffer_size, ssize_t amount){
	char *new_buffer = new char[buffer_size+amount];
	for(ssize_t i = 0; i < buffer_size; i++){
		new_buffer[i] = buffer[i];
	}
	delete[] buffer;
	buffer = new_buffer;
	buffer_size += amount;
}
char *ead_line_from_file(FileID open_source_file){
	ssize_t buffer_size = 8;
	char *source_line = new char[buffer_size];

	int cursor = 0;
	int _read = 0;
	
	char curren_char = ' ';
	while(true){
		_read = read(open_source_file, &curren_char , 1);	
		if(_read == 0){
			log("Reached End of File while Reading a line from FileID:", open_source_file);
			if(cursor == buffer_size){
				_grow_buffer(source_line, buffer_size, 1);
			}
			source_line[cursor] = '\0';

			return source_line;
		}else if(_read == -1){
			err_log("[ERROR] While Reading from FileID :", open_source_file, "\n[ERRNO]", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if(is_new_line(curren_char)){
			if(cursor == buffer_size){
				_grow_buffer(source_line, buffer_size, 1);
			}
			source_line[cursor] = '\0';
			return source_line;
		}
		if(cursor == buffer_size-1){
			_grow_buffer(source_line, buffer_size);
		}
		source_line[cursor] = curren_char ;
		cursor++;
	}

	err_log("[UNKOWN ERROR] REACHED END OF READLINE FUNCTION");
	exit(EXIT_FAILURE);
}

char *read_bytes_from_file(FileID open_source_file, ssize_t read_amount){
	char *source_bytes = new char[read_amount];
	int res = read(open_source_file, source_bytes, read_amount);

	if(res == -1){
		err_log("[ERROR] Failed to read from:", open_source_file);
		err_log("[READ] Read returned:", res);
		err_log("[ERRNO]", strerror(errno));
		exit(EXIT_FAILURE);
	}else if(res == 0){
		std::cout << "[READ] EOF" << "\n";
	}else if(res < read_amount){
		for (int i = res; i < read_amount; i++){
			source_bytes[i] = ' ';
		}
		source_bytes[res] = '\n';
		source_bytes[res+1] = '\0';
	}

	return source_bytes;
}
