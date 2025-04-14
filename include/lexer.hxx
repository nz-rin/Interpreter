#ifndef LEXER_H
#define LEXER_H

#include <cstring>
#include <sys/types.h>
#include <vector>
#include <unordered_map>

#define FileID int

void _grow_buffer(char *&buffer, ssize_t &buffer_size);
char *read_bytes_from_file(FileID open_source_file, ssize_t read_amount);

#endif
