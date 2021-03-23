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
	ast_trees->push_back(new AstNode(AstTag::GLOBAL));
	global(ast_trees->at(0), symbol_table, error_list);
	return ast_trees;
}


std::vector<SyntaxToken>* Parser::get_lexems()
{
	return &_lexems;
}


bool Parser::global(AstNode* global_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// GLOBAL -> PROGRAM
	//		  |  ?
	AstNode* program_node = new AstNode(AstTag::PROGRAM);
	if (program(program_node, symbol_table, error_list))
	{
		global_node->add_child(program_node);
		return true;
	}
	/*else
	{
		delete program_node;
	}*/
	return false;
}


bool Parser::program(AstNode* program_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// PROGRAM	->	PROG_DEF VAR_DECL BLOCK
	AstNode* prog_def_node = new AstNode(AstTag::PROG_DEF);
	if (prog_def(prog_def_node, symbol_table, error_list))
	{
		program_node->add_child(prog_def_node);
		AstNode* var_decl_node = new AstNode(AstTag::VAR_DECL);
		if (var_decl(var_decl_node, symbol_table, error_list))
		{
			program_node->add_child(var_decl_node);
			AstNode* block_node = new AstNode(AstTag::BLOCK);
			if (block(block_node, symbol_table, error_list))
			{
				program_node->add_child(block_node);
			}
			else
			{
				delete block_node;
			}
		}
		else
		{
			delete var_decl_node;
		}
	}
	else
	{
		delete prog_def_node;
		return false;
	}
	return true;
}


bool Parser::prog_def(AstNode* prog_def_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// PROG_DEF	-> PROGRAM ID;
	if (current_token().token_type == SyntaxTag::PROGRAM_TOKEN)
	{
		prog_def_node->add_child(new SyntaxToken(current_token()));
		if (next_token().token_type == SyntaxTag::ID_TOKEN)
		{
			prog_def_node->add_child(new SyntaxToken(current_token()));
			if (next_token().token_type == SyntaxTag::SEMICOLON_TOKEN)
			{
				prog_def_node->add_child(new SyntaxToken(current_token()));
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
		else
		{
			SyntaxToken err_t = current_token();
			error_list.push_back(Error("current file", _error_level,
				ErrorTag::SYNTAX_ERROR,
				"unexpected token, expected \"ID_TOKEN\", but given \"" + err_t.lexeme + "\".",
				err_t.line, err_t.symbol_pos));
		}
	}
	else
	{
		SyntaxToken err_t = current_token();
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected \"PROGRAM_TOKEN\", but given \"" + err_t.lexeme + "\".",
			err_t.line, err_t.symbol_pos));
	}
	return false;
}


bool Parser::var_decl(AstNode* var_decl_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// VAR_DECL	->	VAR DECL
	if (next_token().token_type == SyntaxTag::VAR_TOKEN)
	{
		var_decl_node->add_child(new SyntaxToken(current_token()));
		AstNode* decl_node = new AstNode(AstTag::DECL);
		while (decl(decl_node, symbol_table, error_list))
		{
			var_decl_node->add_child(decl_node);
			decl_node = new AstNode(AstTag::DECL);
		}
		delete decl_node;
		if (lookahead().token_type == SyntaxTag::BEGIN_TOKEN)
			return true;
	}
	else
	{
		SyntaxToken err_t = current_token();
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected \"VAR_TOKEN\", but given \"" + err_t.lexeme + "\".",
			err_t.line, err_t.symbol_pos));
	}
	return false;
}


bool Parser::decl(AstNode* decl_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// DECL	->	ID_LIST : TYPE DECL
	AstNode* id_list_node = new AstNode(AstTag::ID_LIST);
	if (id_list(id_list_node, symbol_table, error_list))
	{
		decl_node->add_child(id_list_node);
		if (next_token().token_type == SyntaxTag::COLON_TOKEN)
		{
			decl_node->add_child(new SyntaxToken(current_token()));
			AstNode* type_node = new AstNode(AstTag::TYPE);
			if (type(type_node, symbol_table, error_list))
			{
				decl_node->add_child(type_node);
				if (next_token().token_type == SyntaxTag::SEMICOLON_TOKEN)
				{
					decl_node->add_child(new SyntaxToken(current_token()));
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
			else
			{
				delete type_node;
			}
		}
		else
		{
			SyntaxToken err_t = current_token();
			error_list.push_back(Error("current file", _error_level,
				ErrorTag::SYNTAX_ERROR,
				"unexpected token, expected \"COLON_TOKEN\", but given \"" + err_t.lexeme + "\".",
				err_t.line, err_t.symbol_pos));
		}
	}
	else
	{
		delete id_list_node;
	}
	return false;
}


bool Parser::id_list(AstNode* id_list_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// ID_LIST	->	, ID ID_LIST
	//			|	EPS
	if (lookahead().token_type == SyntaxTag::ID_TOKEN)
	{
		id_list_node->add_child(new SyntaxToken(next_token()));
		if (lookahead().token_type == SyntaxTag::COMMA_TOKEN)
		{
			id_list_node->add_child(new SyntaxToken(next_token()));
			id_list(id_list_node, symbol_table, error_list);
		}
		return true;
	}
	else
	{
		SyntaxToken err_t = lookahead();
		if (err_t.token_type != SyntaxTag::BEGIN_TOKEN)
			error_list.push_back(Error("current file", _error_level,
				ErrorTag::SYNTAX_ERROR,
				"unexpected token, expected \"ID_TOKEN\", but given \"" + err_t.lexeme + "\".",
				err_t.line, err_t.symbol_pos));
	}
	return false;
}


bool Parser::type(AstNode* type_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// TYPE	->	array[ARR_SIZE] of TYPE
	//		|	BASIC_TYPE
	if (lookahead().token_type == SyntaxTag::ARRAY_TOKEN)
	{
		type_node->add_child(new SyntaxToken(next_token()));
		if (next_token().token_type == SyntaxTag::LSB_TOKEN)
		{
			type_node->add_child(new SyntaxToken(current_token()));
			AstNode* arr_size_node = new AstNode(AstTag::ARR_SIZE);
			if (arr_size(arr_size_node, symbol_table, error_list))
			{
				type_node->add_child(arr_size_node);
				if (next_token().token_type == SyntaxTag::RSB_TOKEN)
				{
					type_node->add_child(new SyntaxToken(current_token()));
					if (next_token().token_type == SyntaxTag::OF_TOKEN)
					{
						type_node->add_child(new SyntaxToken(current_token()));
						AstNode* type_node_nested = new AstNode(AstTag::TYPE);
						if (type(type_node_nested, symbol_table, error_list))
						{
							type_node->add_child(type_node_nested);
							return true;
						}
						else
						{
							delete type_node;
							return false;
						}
					}
					else
					{
						SyntaxToken err_t = current_token();
						error_list.push_back(Error("current file", _error_level,
							ErrorTag::SYNTAX_ERROR,
							"unexpected token, expected \"OF_TOKEN\", but given \"" + err_t.lexeme + "\".",
							err_t.line, err_t.symbol_pos));
						return false;
					}
				}
				else
				{
					SyntaxToken err_t = current_token();
					error_list.push_back(Error("current file", _error_level,
						ErrorTag::SYNTAX_ERROR,
						"unexpected token, expected \"RSB_TOKEN\", but given \"" + err_t.lexeme + "\".",
						err_t.line, err_t.symbol_pos));
					return false;
				}
			}
			else
			{
				delete arr_size_node;
				return false;
			}
		}
		else
		{
			SyntaxToken err_t = current_token();
			error_list.push_back(Error("current file", _error_level,
				ErrorTag::SYNTAX_ERROR,
				"unexpected token, expected \"LSB_TOKEN\", but given \"" + err_t.lexeme + "\".",
				err_t.line, err_t.symbol_pos));
			return false;
		}
	}

	AstNode* basic_type_node = new AstNode(AstTag::BASIC_TYPE);
	if (basic_type(basic_type_node, symbol_table, error_list))
	{
		type_node->add_child(basic_type_node);
		return true;
	}
	else
	{
		delete basic_type_node;
		SyntaxToken err_t = current_token();
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected type declaration, but given \"" + err_t.lexeme + "\".",
			err_t.line, err_t.symbol_pos));
	}
	
	return false;
}


bool Parser::arr_size(AstNode* arr_size_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// ARR_SIZE	-> 1..BYTE_NUMBER
	SyntaxToken one = next_token();
	if (one.lexeme == "1")
	{
		arr_size_node->add_child(new SyntaxToken(one));
		SyntaxToken first_dot = next_token();
		if (first_dot.token_type == SyntaxTag::DOT_TOKEN && next_token().token_type == SyntaxTag::DOT_TOKEN)
		{
			arr_size_node->add_child(new SyntaxToken(first_dot));
			arr_size_node->add_child(new SyntaxToken(current_token()));

			if (next_token().token_type == SyntaxTag::INTEGER_NUMBER)
			{
				// think how to record array size
				arr_size_node->add_child(new SyntaxToken(current_token()));
				return true;
			}
			else
			{
				SyntaxToken err_t = current_token();
				error_list.push_back(Error("current file", _error_level,
					ErrorTag::SYNTAX_ERROR,
					"unexpected token, expected \"INTEGER_NUMBER\", but given \"" + err_t.lexeme + "\".",
					err_t.line, err_t.symbol_pos));
			}
		}
		else
		{
			SyntaxToken err_t = current_token();
			error_list.push_back(Error("current file", _error_level,
				ErrorTag::SYNTAX_ERROR,
				"unexpected token, expected \"DOT_TOKEN\", but given \"" + err_t.lexeme + "\".",
				err_t.line, err_t.symbol_pos));
		}
	}
	else
	{
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected \"1\", but given \"" + one.lexeme + "\".",
			one.line, one.symbol_pos));
	}
	return false;
}


bool Parser::basic_type(AstNode* basic_type_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// BASIC_TYPE -> FLOAT_NUMBER_TYPE | INTEGER_TYPE | BOOL_TYPE | DOUBLE_TYPE | CHAR_TYPE | BYTE_TYPE		
	SyntaxToken token = next_token();
	if (token.token_type == SyntaxTag::FLOAT_TYPE ||
		token.token_type == SyntaxTag::INTEGER_TYPE ||
		token.token_type == SyntaxTag::DOUBLE_TYPE ||
		token.token_type == SyntaxTag::BOOL_TYPE ||
		token.token_type == SyntaxTag::BYTE_TYPE ||
		token.token_type == SyntaxTag::CHAR_TYPE)
	{
		basic_type_node->add_child(new SyntaxToken(token));
		return true;
	}
	else
	{
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected some kind of datatype, but given \"" + token.lexeme + "\".",
			token.line, token.symbol_pos));
	}
	return false;
}


bool Parser::block(AstNode* block_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// BLOCK	->	BEGIN STMT END.
	if (next_token().token_type == SyntaxTag::BEGIN_TOKEN)
	{
		block_node->add_child(new SyntaxToken(current_token()));
		AstNode* stmt_node = new AstNode(AstTag::STMT);
		while (stmt(stmt_node, symbol_table, error_list))
		{
			block_node->add_child(stmt_node);
			stmt_node = new AstNode(AstTag::STMT);
		}
		delete stmt_node;
		
		if (next_token().token_type == SyntaxTag::END_TOKEN)
		{
			block_node->add_child(new SyntaxToken(current_token()));
			if (next_token().token_type == SyntaxTag::DOT_TOKEN)
			{
				block_node->add_child(new SyntaxToken(current_token()));
				return true;
			}
			else
			{
				SyntaxToken err_t = current_token();
				error_list.push_back(Error("current file", _error_level,
					ErrorTag::SYNTAX_ERROR,
					"unexpected token, expected \"DOT_TOKEN\", but given \"" + err_t.lexeme + "\".",
					err_t.line, err_t.symbol_pos));
			}
		}
		else
		{
			SyntaxToken err_t = current_token();
			error_list.push_back(Error("current file", _error_level,
				ErrorTag::SYNTAX_ERROR,
				"unexpected token, expected \"END_TOKEN\", but given \"" + err_t.lexeme + "\".",
				err_t.line, err_t.symbol_pos));
		}
	}
	else
	{
		SyntaxToken err_t = current_token();
		error_list.push_back(Error("current file", _error_level,
			ErrorTag::SYNTAX_ERROR,
			"unexpected token, expected \"BEGIN_TOKEN\", but given \"" + err_t.lexeme + "\".",
			err_t.line, err_t.symbol_pos));
	}

	return false;
}


bool Parser::stmt(AstNode* stmt_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// STMT		->	ID := EXPR;
	//			|	EPS
	if (lookahead().token_type == SyntaxTag::ID_TOKEN)
	{
		// adding element in symbol table
		if (symbol_table.search(next_token().lexeme) == symbol_table.get_size())
			symbol_table.insert(current_token().lexeme, current_token()._value, SyntaxTag::FLOAT_NUMBER);
		//
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
			else
			{
				delete expr_node;
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
		else
			delete add_sub_node;
		return true;
	}
	else
	{
		delete trans_node;
	}
	return false;
}


bool Parser::add_sub(AstNode* add_sub_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// ADD_SUB		->		+ TRANS ADD_SUB
	//				|		- TRANS ADD_SUB
	//				|		EPS
	SyntaxToken word = lookahead();
	if (word.token_type == SyntaxTag::PLUS_TOKEN || word.token_type == SyntaxTag::MINUS_TOKEN)
	{
		add_sub_node->add_child(new SyntaxToken(next_token()));
		AstNode* trans_node = new AstNode(AstTag::TRANS);
		if (trans(trans_node, symbol_table, error_list))
		{
			add_sub_node->add_child(trans_node);
			AstNode* add_sub_node_nested = new AstNode(AstTag::ADD_SUB);
			if (add_sub(add_sub_node_nested, symbol_table, error_list))
				add_sub_node->add_child(add_sub_node_nested);
			else
				delete add_sub_node_nested;
			return true;
		}
		else
		{
			delete trans_node;
		}
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
		else
			delete mul_div_node;
		return true;
	}
	else
	{
		delete factor_node;
	}
	return false;
}


bool Parser::mul_div(AstNode* mul_div_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// MUL_DIV -> * FACTOR MUL_DIV
	//			| / FACTOR MUL_DIV
	//			| EPS
	SyntaxToken word = lookahead();
	if (word.token_type == SyntaxTag::STAR_TOKEN || word.token_type == SyntaxTag::SLASH_TOKEN)
	{
		mul_div_node->add_child(new SyntaxToken(next_token()));
		AstNode* factor_node = new AstNode(AstTag::FACTOR);
		if (factor(factor_node, symbol_table, error_list))
		{
			mul_div_node->add_child(factor_node);
			AstNode* mul_div_node_nested = new AstNode(AstTag::MUL_DIV);
			if (mul_div(mul_div_node_nested, symbol_table, error_list))
				mul_div_node->add_child(mul_div_node_nested);
			else
				delete mul_div_node_nested;
			return true;
		}
		else
		{
			delete factor_node;
		}
	}
	return false;
}


bool Parser::factor(AstNode* factor_node, hash_map& symbol_table, std::vector<Error>& error_list)
{
	// FACTOR	->	(EXPR)
	//			|	FLOAT_NUMBER | INTEGER_NUMBER | DOUBLE_NUMBER | CHAR_LITERAL | FALSE | TRUE
	//			|	ID
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
		else
		{
			delete expr_node;
		}
	}
	else if (current_token().token_type == SyntaxTag::DOUBLE_NUMBER
		|| current_token().token_type == SyntaxTag::FLOAT_NUMBER
		|| current_token().token_type == SyntaxTag::INTEGER_NUMBER
		|| current_token().token_type == SyntaxTag::CHAR_LITERAL
		|| current_token().token_type == SyntaxTag::TRUE
		|| current_token().token_type == SyntaxTag::FALSE)
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
		"unexpected token, expected \"ID_TOKEN\"\\some literal\\\"LP_TOKEN\", but given \"" + err_t.lexeme + "\".",
		err_t.line, err_t.symbol_pos));
	return false;
}

