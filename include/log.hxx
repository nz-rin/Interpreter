#ifndef LOG_H
#define LOG_H

#include <iostream>

template<typename T>
void log(T &&arg){
	std::cout << arg << "\n";
}

template<typename T, typename... args>
void log(T &&first, args &&...rest){
	std::cout << first << " ";
	log(std::forward<args>(rest)...);
}

template<typename T>
void err_log(T &&arg){
	std::cerr << arg << "\n";
}

template<typename T>
void err_log_and_exit(T &&arg){
	std::cerr << arg << "\n";
	exit(EXIT_FAILURE);
}
template<typename T, typename... args>
void err_log(T &&first, args &&...rest){
	std::cerr << first << " ";
	err_log(std::forward<args>(rest)...);
}

template<typename T, typename... args>
void err_log_and_exit(T &&first, args &&...rest){
	std::cerr << first << " ";
	err_log(std::forward<args>(rest)...);
	exit(EXIT_FAILURE);
}

#ifdef DLOG
	template<typename T>
	void d_log(T &&arg){
		std::cout << arg << "\n";
	}

	template<typename T, typename... args>
	void d_log(T &&first, args &&...rest){
		std::cout << first << " ";
		log(std::forward<args>(rest)...);
	}
#else
	template<typename T>
	void d_log(T &&arg){
		(void)arg;
	}

	template<typename T, typename... args>
	void d_log(T &&first, args &&...rest){
		(void)first;
		(void)(sizeof...(rest));
	}

#endif

#endif
