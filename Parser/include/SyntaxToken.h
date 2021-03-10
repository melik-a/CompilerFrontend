#pragma once

#include <string>

#include "AstNode.h"
#include "SyntaxTag.h"



struct SyntaxToken : AstNode
{
	std::string lexeme;
	SyntaxTag token_type;

	size_t line;
	size_t symbol_pos;

	SyntaxToken() = default;

	SyntaxToken(const SyntaxToken& tok) :
		lexeme(tok.lexeme), token_type(tok.token_type), line(tok.line), symbol_pos(tok.symbol_pos) {}

	SyntaxToken(std::string lex, SyntaxTag lex_type)
		: lexeme(lex), token_type(lex_type), line(), symbol_pos() {}

	SyntaxToken(std::string lex, SyntaxTag lex_type, size_t f_line, size_t f_pos)
		: lexeme(lex), token_type(lex_type), line(f_line), symbol_pos(f_pos) {}

	void print_node() override
	{
		std::cout << " <" << token_type << ", " << lexeme << "> ";
	}

};