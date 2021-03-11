#include "../include/Parser.h"


SyntaxToken Parser::peek_token(size_t pos) const
{
	size_t last = _lexems.size() - 1;
	if (pos <= last)
		return _lexems[last];
	return _lexems[pos];
}

SyntaxToken Parser::current_token() const
{
	return _lexems[_current];
}

SyntaxToken Parser::next_token()
{
	if (_current + 1 >= _lexems.size())
		return _lexems[_current];
	return _lexems[++_current];
}

SyntaxToken Parser::lookahead() const
{
	if (_current + 1 >= _lexems.size())
		return _lexems[_current];
	return _lexems[_current + 1];
}


std::vector<AstNode*>* Parser::parse(hash_map& symbol_table, std::vector<Error>& error_list)
{
	std::vector<AstNode*>* ast_trees = new std::vector<AstNode*>;
	for (size_t line = 0; line < _lines; line++)
	{
		ast_trees->push_back(new AstNode(AstTag::STMT));
		stmt(ast_trees->at(line), symbol_table, error_list);
	}
	return ast_trees;
}


std::vector<SyntaxToken>* Parser::get_lexems()
{
	return &_lexems;
}


bool Parser::stmt(AstNode* stmt_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// STMT -> ID := EXPR;
	if (current_token().token_type == SyntaxTag::ID_TOKEN)
	{
		if (symbol_table.search(current_token().lexeme) == symbol_table.get_size())
			symbol_table.insert(current_token().lexeme, current_token()._value, SyntaxTag::FLOAT_NUMBER);
		stmt_node->add_child(new SyntaxToken(current_token()));
		if (next_token().token_type == SyntaxTag::ASSIGN_TOKEN)
		{
			stmt_node->add_child(new SyntaxToken(current_token()));
			AstNode* expr_node = new AstNode(AstTag::EXPRESSION);
			if (expr(expr_node, symbol_table, error_list))
			{
				stmt_node->add_child(expr_node);
				if (next_token().token_type == SyntaxTag::SEMICOLON_TOKEN)
				{
					stmt_node->add_child(new SyntaxToken(current_token()));
					next_token();
					return true;
				}
				else
				{
					SyntaxToken err_t = current_token();
					error_list.push_back(Error("current file", _error_level,
						ErrorTag::SYNTAX_ERROR,
						"unexpected token, expected \"SEMICOLON_TOKEN\", but given \"" + err_t.lexeme + "\".",
						err_t.line, err_t.symbol_pos));
				}
			}
		}
		else
		{
			SyntaxToken err_t = current_token();
			error_list.push_back(Error("current file", _error_level,
				ErrorTag::SYNTAX_ERROR,
				"unexpected token, expected \"ASSIGN_TOKEN\", but given \"" + err_t.lexeme + "\".",
				err_t.line, err_t.symbol_pos));
		}
	}
	else
	{
		SyntaxToken err_t = current_token();
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected \"ID_TOKEN\", but given \"" + err_t.lexeme + "\".",
			err_t.line, err_t.symbol_pos));
	}
	return false;
}

bool Parser::expr(AstNode* expr_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// EXPR -> TRANS ADD_SUB
	AstNode* trans_node = new AstNode(AstTag::TRANS);
	if (trans(trans_node, symbol_table, error_list))
	{
		expr_node->add_child(trans_node);
		AstNode* add_sub_node = new AstNode(AstTag::ADD_SUB);
		if (add_sub(add_sub_node, symbol_table, error_list))
			expr_node->add_child(add_sub_node);
		return true;
	}
	return false;
}

bool Parser::add_sub(AstNode* add_sub_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// ADD_SUB -> + TRANS ADD_SUB
	SyntaxToken word = lookahead();
	if (word.token_type == SyntaxTag::PLUS_TOKEN || word.token_type == SyntaxTag::MINUS_TOKEN)
	{
		add_sub_node->add_child(new SyntaxToken(current_token()));
		next_token();
		AstNode* trans_node = new AstNode(AstTag::TRANS);
		if (trans(trans_node, symbol_table, error_list))
		{
			add_sub_node->add_child(trans_node);
			AstNode* add_sub_node_nested = new AstNode(AstTag::ADD_SUB);
			if (add_sub(add_sub_node_nested, symbol_table, error_list))
				add_sub_node->add_child(add_sub_node_nested);
			return true;
		}
	}
	else
	{
		SyntaxToken err_t = current_token();
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected \"MINUS_TOKEN\" or \"PLUS_TOKEN\", but given \"" + err_t.lexeme + "\".",
			err_t.line, err_t.symbol_pos));
	}
	return false;
}

bool Parser::trans(AstNode* trans_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// TRANS -> FACTOR MUL_DIV
	AstNode* factor_node = new AstNode(AstTag::FACTOR);
	if (factor(factor_node, symbol_table, error_list))
	{
		trans_node->add_child(factor_node);
		AstNode* mul_div_node = new AstNode(AstTag::MUL_DIV);
		if (mul_div(mul_div_node, symbol_table, error_list))
			trans_node->add_child(mul_div_node);
		return true;
	}
	return false;
}

bool Parser::mul_div(AstNode* mul_div_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// MUL_DIV -> FACTOR MUL_DIV
	SyntaxToken word = lookahead();
	if (word.token_type == SyntaxTag::STAR_TOKEN || word.token_type == SyntaxTag::SLASH_TOKEN)
	{
		mul_div_node->add_child(new SyntaxToken(current_token()));
		next_token();
		AstNode* factor_node = new AstNode(AstTag::FACTOR);
		if (factor(factor_node, symbol_table, error_list))
		{
			mul_div_node->add_child(factor_node);
			AstNode* mul_div_node_nested = new AstNode(AstTag::MUL_DIV);
			if (mul_div(mul_div_node_nested, symbol_table, error_list))
				mul_div_node->add_child(mul_div_node_nested);
			return true;
		}
	}
	else 
	{
		SyntaxToken err_t = current_token();
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected \"STAR_TOKEN\" or \"SLASH_TOKEN\", but given \"" + err_t.lexeme + "\".",
			err_t.line, err_t.symbol_pos));
	}
	return false;
}

bool Parser::factor(AstNode* factor_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// FACTOR -> ( EXPR ) | FLOAT_NUM | ID_TOKEN
	if (next_token().token_type == SyntaxTag::LP_TOKEN)
	{
		factor_node->add_child(new SyntaxToken(current_token()));
		AstNode* expr_node = new AstNode(AstTag::EXPRESSION);
		if (expr(expr_node, symbol_table, error_list))
		{
			factor_node->add_child(expr_node);
			if (next_token().token_type == SyntaxTag::RP_TOKEN)
			{
				factor_node->add_child(new SyntaxToken(current_token()));
				return true;
			}
			else
			{
				SyntaxToken err_t = current_token();
				error_list.push_back(Error("current file", _error_level,
					ErrorTag::SYNTAX_ERROR,
					"unexpected token, expected \"RP_TOKEN\", but given \"" + err_t.lexeme + "\".",
					err_t.line, err_t.symbol_pos));
			}
		}
	}
	else if (current_token().token_type == SyntaxTag::FLOAT_NUMBER)
	{
		factor_node->add_child(new SyntaxToken(current_token()));
		return true;
	}
	else if (current_token().token_type == SyntaxTag::ID_TOKEN)
	{
		if (symbol_table.search(current_token().lexeme) == symbol_table.get_size())
			symbol_table.insert(current_token().lexeme, current_token()._value, SyntaxTag::FLOAT_NUMBER);
		factor_node->add_child(new SyntaxToken(current_token()));
		return true;
	}
	SyntaxToken err_t = current_token();
	error_list.push_back(Error("current file", _error_level,
		ErrorTag::SYNTAX_ERROR,
		"unexpected token, expected \"ID_TOKEN\"\\\"FLOAT_NUMBER\"\\\"LP_TOKEN\", but given \"" + err_t.lexeme + "\".",
		err_t.line, err_t.symbol_pos));
	return false;
}

