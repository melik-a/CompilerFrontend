#pragma once


#include "Parser/include/AstDrawer.h"
#include "Parser/include/AstNode.h"
#include "Parser/include/Parser.h"

#include "Lexer/Scanner.h"

#include "SymbolTable/HashMap/HashMap.h"

struct ComplexFront
{
	~ComplexFront()
	{
		for (auto& el : *_parsed_ast)
		{
			if (el)
				delete el;
		}
		delete _parsed_ast;
		delete _lexems;
	}

	void front_compile(std::string file);
	void draw_all_trees();
	void print_parsed_lines();
	void print_symbol_table();
	void print_line_and_tree();

	private:
		std::vector<AstNode*>* _parsed_ast;
		std::vector<SyntaxToken>* _lexems;
		HashMap<std::string, std::string> _symbol_table;
};