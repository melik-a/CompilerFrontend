#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "States.h"
#include "../Parser/include/SyntaxToken.h"
#include "../Parser/include/SyntaxTag.h"

struct LexicalScanner
{
	LexicalScanner() = delete;
	LexicalScanner(std::string& file);
	~LexicalScanner();

	std::vector<SyntaxToken>* scan();
	size_t get_num_of_lines();

	private:
		std::vector<SyntaxToken>* _lexeme_table;
		size_t _line_counter{1};
		size_t _symbol_pos_at_line{1};
		std::ifstream _file;
		States _current_state;

		void start_state_changing(char symbol, std::string& lexeme);
		void id_state_changing(char symbol, std::string& lexeme);
		void assignment_state_changing(char symbol, std::string& lexeme);
		void float_num_state_changing(char symbol, std::string& lexeme);
		void separator_state_changing(char symbol, std::string& lexeme);
		void arithm_operator_state_changing(char symbol, std::string& lexeme);
		void comment_state_changing(char symbol, std::string& lexeme);

		bool is_letter(char symbol) const;
		bool is_digit(char symbol) const;
};

