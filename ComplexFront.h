#pragma once


#include "Parser/include/AstDrawer.h"
#include "Parser/include/AstNode.h"
#include "Parser/include/Parser.h"

#include "Lexer/Scanner.h"

#include "SymbolTable/HashMap/HashMap.h"

struct ComplexFront
{
	~ComplexFront();

	void front_compile(std::string file);
	void draw_all_trees();
	void print_parsed_lines();
	void print_symbol_table();
	void print_line_and_tree();

	private:
		std::vector<AstNode*>* _parsed_ast { nullptr };
		std::vector<SyntaxToken>* _lexems { nullptr };
		HashMap<std::string, float>* _symbol_table { nullptr };
};