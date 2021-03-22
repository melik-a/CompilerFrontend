#pragma once


#include "Parser/include/AstDrawer.h"
#include "Parser/include/AstNode.h"
#include "Parser/include/Parser.h"

#include "Lexer/Scanner.h"

#include "SymbolTable/HashMap/HashMap.h"

#include "Errors/Error.h"

struct ComplexFront
{
	~ComplexFront();

	bool front_compile(std::string file);
	void draw_all_trees();
	void print_parsed_lines();
	void print_symbol_table();
	void print_error_list();

	private:
		std::vector<Error>* _errors;
		std::vector<AstNode*>* _parsed_ast { nullptr };
		std::vector<SyntaxToken>* _lexems { nullptr };
		HashMap<std::string, float>* _symbol_table { nullptr };
};