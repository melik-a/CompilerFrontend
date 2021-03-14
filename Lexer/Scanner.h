#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

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
	size_t get_num_of_codelines();
	

	private:
		std::vector<SyntaxToken>* _lexeme_table;
		
		size_t _current_pos{ 0 };
		std::string _current_line;
		size_t _line_counter{1};
		size_t _code_line_counter{1};

		std::string _file_name;
		std::ifstream _file;
		static const unsigned int _error_level = 0x0000B001;

		void id_lexeme_scan();
		void number_lexeme_scan();
		void assign_or_colon_scan();
		void ignore_comment(std::vector<Error>& error_list);

		void a_lexemes_scan();
		void b_lexemes_scan();
		void c_lexemes_scan();
		void d_lexemes_scan();
		void e_lexemes_scan();
		void f_lexemes_scan();
		void i_lexemes_scan();
		void o_lexemes_scan();
		void p_lexemes_scan();
		void t_lexemes_scan();
		void v_lexemes_scan();
		
		void char_literal_lexeme_scan();

		bool is_letter(char symbol) const;
		bool is_digit(char symbol) const;
};

