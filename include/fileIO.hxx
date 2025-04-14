#ifndef FILEIO_H
#define FILEIO_H

#include <sys/types.h>
#include <string>

#define FileID int

FileID open_file(const char *file_path_and_name);
FileID create_file(const char *file_path_and_name);

void close_file(FileID open_file);
void write_to_file(FileID open_file, void *file_data, size_t data_size);

std::string read_line_from_file(FileID source_file);
std::string read_all_from_file(FileID source_file);

void log_file_states();
bool get_file_state(FileID File);

void init_memory();
void cleanup_memory();
#endif
