#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "../Parser/include/SyntaxToken.h"
#include "../Parser/include/SyntaxTag.h"
#include "../Errors/Error.h"

struct LexicalScanner
{
	LexicalScanner() = delete;
	LexicalScanner(std::string& file);
	~LexicalScanner();

	std::vector<SyntaxToken>* scan(std::vector<Error>& error_list);
	size_t get_num_of_lines();

	private:
		std::vector<SyntaxToken>* _lexeme_table;
		std::unordered_map<std::string, SyntaxTag> _keywords;

		size_t _current_pos{0};
		std::string _current_line;
		size_t _line_counter{1};

		std::string _file_name;
		std::ifstream _file;
		static const unsigned int _error_level = 0x0000B001;

		void id_lexeme_scan();
		void number_lexeme_scan();
		void assign_or_colon_scan();
		void ignore_comment(std::vector<Error>& error_list);
		
		void char_literal_lexeme_scan(std::vector<Error>& error_list);

		bool is_letter(char symbol) const;
		bool is_digit(char symbol) const;
};

