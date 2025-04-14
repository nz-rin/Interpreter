#ifndef STACK_H
#define STACK_H

#include <sys/types.h>
#include <vector>

//stack 
class Stack{
	private:
	unsigned char *stack;

	size_t stack_size;
	size_t stack_pointer;

	const static size_t m_long_size = sizeof(size_t);
	const static size_t m_int_size = sizeof(int);
	const static size_t m_short_size = sizeof(short);
	const static size_t m_byte_size = sizeof(char);


	bool _stack_pointer_valid_underflow(size_t data_size);
	bool _stack_pointer_valid_overflow(size_t data_size);

	void _check_stack_underflow(size_t data_size);
	void _check_stack_overflow(size_t data_size);

	public:
	Stack(size_t stack_size);
	~Stack();

	size_t get_stack_pointer();

	void push(unsigned char byte_len, const char *byte_data);
	void push(const char *byte_data);
	void push_s(const char *byte_data);
	void push_i(const char *byte_data);
	void push_l(const char *byte_data);
	/*
	void push_i(int int_data);
	void push_l(ssize_t ssize_t_data);
	void push_s(short short_data);
	*/
	char *pop(unsigned char data_lenght);
	char *pop();
	char *pop_s();
	char *pop_i();
	char *pop_l();

	char read();
	size_t get_SP();

	void dump();
};

#endif
