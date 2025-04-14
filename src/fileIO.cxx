#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iomanip>
#include <unistd.h>
#include <vector>

#include "log.hxx"
#include "utils.hxx"

#include "fileIO.hxx"


#define line_buffer_size 128
#define file_buffer_size 1024

// TODO clean up for memory
char *m_file_buffer = nullptr;
char *m_line_buffer = nullptr;

struct file_record{
	std::string file_name;
	FileID file;
	bool open;
	
	file_record(const char* file_name, FileID file, bool open){
		this->file_name = file_name;
		this->file = file;
		this->open = open;
	}
};

std::vector<file_record> file_record_keeper = std::vector<file_record>();
std::unordered_map<std::string_view, int> file_map = std::unordered_map<std::string_view, int>();

FileID open_file(const char *file_path_and_name){

	if(file_map.find(file_path_and_name) != file_map.end()){
		for(size_t i = 0; i < file_record_keeper.size(); i++){
			auto _ = file_record_keeper[i];

			if(_.file == file_map.at(file_path_and_name)){
				if(_.open){
					log("File", file_path_and_name, "is already open, returning previous ID");
					return _.file;
				}else{
					log("File", file_path_and_name, "was open previous but has been closed, Reopening File");
					file_map.erase(_.file_name);
					file_record_keeper.erase(file_record_keeper.cbegin() + i);
					break;
				}
			}
		}
	}

	int file = open(file_path_and_name, O_RDONLY);
	if(file == -1){
		err_log("[OPEN] Failed to open: ", *file_path_and_name);
		err_log("[ERRNO]", strerror(errno));
		exit(EXIT_FAILURE);
	}
	file_map[file_path_and_name] = file;
	file_record_keeper.push_back(file_record(file_path_and_name, file, true));

	return file;
}

void close_file(FileID open_file){
	int _ = close(open_file);
	if(_ == -1){
		err_log_and_exit("[FILEIO ERROR] Failed to close FileID:", open_file, "\n[ERRNO]", strerror(errno));
	}
	
	for(size_t i = 0; i < file_record_keeper.size(); i++){
		if(file_record_keeper[i].file == open_file){
			file_record_keeper[i].open = false;	
			break;
		}
	}

}

FileID create_file(const char *file_name_and_path){
	FileID fileID = open(file_name_and_path, O_WRONLY | O_APPEND | O_CREAT, 0644);

	if(fileID == -1){
		err_log_and_exit("[FILEIO ERROR] Failed to create file: ", file_name_and_path, "\n[ERRNO]", strerror(errno));
	}
	file_map[file_name_and_path] = fileID;
	file_record_keeper.push_back(file_record(file_name_and_path, fileID, true));
	return fileID;
}

void write_to_file(FileID open_file, void *file_data, size_t data_size){
	ssize_t _ = write(open_file, file_data, data_size);
	
#ifdef LOG
	//DEBUG INFO
	log("[Write Called]");
	char *written = new char[data_size];
	memcpy(written, file_data, data_size);
	printf("    [WROTE] ");
	for(size_t i = 0; i < data_size; i++){
		std::cout<< std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<unsigned int>(static_cast<unsigned char*>(file_data)[i]) << " ";
	}
	std::cout << "\n";
	delete[] written;	
#endif
	//ERROR DETECTION
	if(_ != static_cast<ssize_t>(data_size)){
		err_log("[WARNING] FileID :", open_file, "\nExepcted to write", data_size, "bytes to file but ", _, " bytes where written instead");
	}else if( _ == static_cast<ssize_t>(-1) ){
		err_log_and_exit("[FILEIO ERROR] Failed to write to FileID:", open_file, "\n[ERRNO]", strerror(errno));
	}

}


std::string read_line_from_file(FileID source_file){
	if(!get_file_state(source_file)){
		err_log_and_exit("[FILE] file not open");
	}

	std::string _return = "";
	size_t _read;
	
	while(true){
		_read = read(source_file, m_line_buffer, line_buffer_size);	
		if(_read == 0){
			log("Reached End of File while Reading a line from FileID:", source_file);
			return _return;
		}else if(_read == static_cast<size_t>(-1)){
			err_log_and_exit("[ERROR] While Reading from FileID :", source_file, "\n[ERRNO]", strerror(errno));
		}
		if(_read < line_buffer_size){
			_return.append(m_line_buffer,_read-1);
			return _return;
		}
		for(size_t i = 0; i < line_buffer_size; i++){
			if(m_line_buffer[i] == '\n' || m_line_buffer[i] == '\0'){
				_return.append(m_line_buffer,--i);
				return _return;
			}
		}
		_return.append(m_line_buffer);

	}
}

std::string read_all_from_file(FileID source_file){
	if(!get_file_state(source_file)){
		err_log_and_exit("[FILE] file not open");
	}

	std::string file_data = "";
	size_t _read = 0;

	while(true){
		_read = read(source_file, m_file_buffer, file_buffer_size);	
		if(_read == 0){
			log("[WARNING] Reached End of File while Reading a line from FileID: ", source_file, " Returnig Empty string" );
			file_data.clear();
			return file_data;
		}else if(_read == static_cast<size_t>(-1)){
			err_log_and_exit("[ERROR] While Reading from FileID :", source_file, "\n[ERRNO]", strerror(errno));
		}
		if(_read < file_buffer_size){
			log("Read less than buffer size: ", _read );
			file_data.append(m_file_buffer,_read);
			return file_data;
		}

		file_data.append(m_file_buffer);

	}

}
void init_memory(){
	m_file_buffer = new char[file_buffer_size];
	m_line_buffer = new char[line_buffer_size];
	file_record_keeper.reserve(16);
	file_map.reserve(16);
}
void cleanup_memory(){
	log("SIZES");
	log("[file Map]", file_map.size());
	log("[Rec keep]", file_record_keeper.size());
	delete[] m_file_buffer;
	delete[] m_line_buffer;
	m_file_buffer = nullptr;
	m_line_buffer = nullptr;
}

bool get_file_state(FileID File){
	for( auto _ : file_record_keeper){
		if(_.file == File){
			return _.open;
		}
	}
	err_log("[WARNING] File:", File, " is not a valid id");
	return false;
}

void log_file_states(){
	for( auto _ : file_record_keeper){
		if(_.open){
			log("[FILE]", _.file_name, " is open");
		}else{
			log("[FILE]", _.file_name, " is closed");
		}
	}
}
