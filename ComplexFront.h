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
	}

	void code_to_ast(std::string file);
	void draw_ast();

	private:
		std::vector<AstNode*>* _parsed_ast;
		//HashMap<std::string, std::string> _symbol_table;
};