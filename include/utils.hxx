#ifndef UTILS_H
#define UTILS_H

#include "data.hxx"
#include <cstring>
#include <sys/types.h>

#define cast(pointer, r_type) *reinterpret_cast<r_type>(pointer)

namespace adrcast{
	inline char* Char(void *pointer){
		return reinterpret_cast<char*>(pointer);
	}

};
namespace pcast{
	inline short Short(void *pointer){
		return *reinterpret_cast<short *>(pointer);	
	}

	inline int Int(void *pointer){
		return *reinterpret_cast<int *>(pointer);	
	}

	inline char Char(void *pointer){
		return *reinterpret_cast<char *>(pointer);
	}

	inline ssize_t Long(void *pointer){
		return *reinterpret_cast<ssize_t *>(pointer);
	}
	inline float Float(void *pointer){
		return *reinterpret_cast<float *>(pointer);
	}
	inline double Double(void *pointer){
		return *reinterpret_cast<double *>(pointer);
	}
}

namespace sret{
	inline short Short(const char *pointer){
		short local;
		memcpy(&local, pointer, sizes::SHORT);
		return local;
	}

	inline int Int(const char *pointer){
		int local;
		memcpy(&local, pointer, sizes::INT);
		return local;
	}

	inline ssize_t Long(const char *pointer){
		ssize_t local;
		memcpy(&local, pointer, sizes::LONG);
		return local;
	}
}

#endif
