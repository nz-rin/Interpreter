#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sys/types.h>

#include "log.hxx"
#include "data.hxx"

#include "stack.hxx"

Stack::Stack(size_t stack_size){
	if(stack_size <= 0){
		err_log_and_exit("[STACK ERROR] Size:", stack_size, " Is invalid, size needs to be greater than 0" );
	}

	this->stack_size = stack_size;
	this->stack = new unsigned char[stack_size];	
	this->stack_pointer = 0;
}

Stack::~Stack(){
	delete[] this->stack;
}

bool Stack::_stack_pointer_valid_overflow(size_t data_size){
	return !(stack_pointer <= stack_size - data_size);
}
bool Stack::_stack_pointer_valid_underflow(size_t data_size){
	return !(stack_pointer >= data_size);
}

void Stack::_check_stack_overflow(size_t data_size){
	if(_stack_pointer_valid_overflow(data_size)){
		err_log_and_exit("[STACK ERROR] Stack overflow, Attempted to add", data_size, " bytes at SP ", stack_pointer,  " with a stack size of ", stack_size);
	}
}

void Stack::_check_stack_underflow(size_t data_size){
	if(_stack_pointer_valid_underflow(data_size)){
		err_log_and_exit("[STACK ERROR] Stack underflow, Attempted to pop", data_size, "bytes from Stack when SP is", stack_pointer);
	}
}

size_t Stack::get_stack_pointer(){
	return stack_pointer;
}

//#################################################################################################

void Stack::push(unsigned char data_len, const char *byte_data){
	switch (data_len){
		case dType::BIT8:
			Stack::push(byte_data);
			break;
		case dType::BIT16:
			Stack::push_s(byte_data);
			break;
		case dType::BIT32:
			Stack::push_i(byte_data);
			break;
		case dType::BIT64:
			Stack::push_l(byte_data);
			break;
		default:
			err_log_and_exit("[STACK ERROR] Cannot push byte of lenght", (int) data_len, "onto the stack");
	}
}
void Stack::push(const char *char_data){
	_check_stack_overflow(Stack::m_byte_size);
	stack[stack_pointer] = *char_data;
	stack_pointer++;
}

void Stack::push_s(const char *byte_data){
	_check_stack_overflow(Stack::m_short_size);
	memcpy(&stack[stack_pointer], byte_data, Stack::m_short_size);
	stack_pointer += Stack::m_short_size;
}

void Stack::push_i(const char *byte_data){
	_check_stack_overflow(Stack::m_int_size);
	memcpy(&stack[stack_pointer], byte_data, Stack::m_int_size);
	stack_pointer += Stack::m_int_size;
}

void Stack::push_l(const char *byte_data){
	_check_stack_overflow(Stack::m_long_size);
	memcpy(&stack[stack_pointer], byte_data, Stack::m_long_size);
	stack_pointer += Stack::m_long_size;
}
/*
void Stack::push_l(ssize_t ssize_t_data){
	_check_stack_overflow(Stack::m_long_size);
	memcpy(&stack[stack_pointer], &ssize_t_data, Stack::m_long_size);
	stack_pointer += Stack::m_long_size;
}

void Stack::push_i(int int_data){
	_check_stack_overflow(Stack::m_int_size);
	memcpy(&stack[stack_pointer], &int_data, Stack::m_int_size);
	stack_pointer += Stack::m_int_size;
}

void Stack::push_s(short short_data){
	_check_stack_overflow(Stack::m_short_size);
	memcpy(&stack[stack_pointer], &short_data, Stack::m_short_size);
	stack_pointer += Stack::m_short_size;
}
*/
char *Stack::pop(unsigned char data_length){
	switch(data_length){
		case dType::BIT8:
			return pop();
		case dType::BIT16:
			return pop_s();
		case dType::BIT32:
			return pop_i();
		case dType::BIT64:
			return pop_l();
		default:
			err_log_and_exit("[DEFAULT CASE]\n  [STACK ERROR] Cannot pop", (int) data_length, "of the Stack, ");
			//to shut the compiler up
			return new char();
	};
}

char *Stack::pop(){
	_check_stack_underflow(Stack::m_byte_size);
	//char *_return_byte = new char(stack[stack_pointer-1]) ;
	stack_pointer--;

	//return _return_byte;
	return (char*)&stack[stack_pointer];
}

char *Stack::pop_s(){
	_check_stack_underflow(Stack::m_short_size);
	//char *_return_bytes = new char[Stack::m_short_size];
	stack_pointer -= Stack::m_short_size;
	//memcpy(_return_bytes, &stack[stack_pointer],Stack::m_short_size);
	//return _return_bytes;
	return (char*)&stack[stack_pointer];
}

char *Stack::pop_i(){
	_check_stack_underflow(Stack::m_int_size);
	//char *_return_bytes = new char[Stack::m_int_size];
	stack_pointer -= Stack::m_int_size;
	//memcpy(_return_bytes, &stack[stack_pointer],Stack::m_int_size);
	//return _return_bytes;
	return (char*)&stack[stack_pointer];
}

char *Stack::pop_l(){
	_check_stack_underflow(Stack::m_long_size);
	//char *_return_bytes = new char[Stack::m_long_size];
	stack_pointer -= Stack::m_long_size;
	//memcpy(_return_bytes, &stack[stack_pointer],Stack::m_long_size);
	//return _return_bytes;
	return (char*)&stack[stack_pointer];
}

//#################################################################################################

char Stack::read(){
	return stack[stack_pointer];
}

void Stack::dump(){
	log("[STACK DUMP]");
	for(size_t i = 0; i < stack_size-4; i += 4){
		std::cout 
		<< std::hex << std::setfill('0') << std::setw(8)
		<< i << "    :    "
		<< std::setfill('0') << std::setw(2) << (int)stack[i] << " "
		<< std::setfill('0') << std::setw(2) << (int)stack[i+1] << " "
		<< std::setfill('0') << std::setw(2) << (int)stack[i+2] << " "
		<< std::setfill('0') << std::setw(2) << (int)stack[i+3]  
		<< "\n" ;
	} 
	std::cout << std::dec;
}

size_t Stack::get_SP(){
	return stack_pointer;
}
