#pragma once

#include <string>
#include <iostream>

#include "ErrorTag.h"


struct Error
{
	Error() = delete;
	Error(std::string filename, int error_code, ErrorTag type, std::string message, size_t line, size_t pos) :
		_filename(filename),_code(error_code), _type(type), _message(message), _line(line), _pos(pos) {}

	void print()
	{
		std::cout << _code;
		std::cout << " |";
		std::cout << "\t";
		std::cout.width(15);
		std::cout.fill(' ');
		std::cout << std::left << _type;
		std::cout << "| ";
		std::cout.width(100);
		std::cout.fill(' ');
		std::cout << std::left << _message;
		std::cout << "| ";
		std::cout.width(15);
		std::cout.fill(' ');
		std::cout << _filename;
		std::cout << "| ";
		std::cout.width(4);
		std::cout.fill(' ');
		std::cout << _line;
		std::cout << "| ";
		std::cout << _pos;
		std::cout << "|\n";
	}

	private:
		unsigned int _code;
		ErrorTag _type;
		std::string _message;
		std::string _filename;
		size_t _line;
		size_t _pos;
};
