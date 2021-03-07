#include "../include/Parser.h"
#include <fstream>


SyntaxToken Parser::peek_token(size_t pos)
{
	size_t last = _lexems.size() - 1;
	if (pos <= last)
		return _lexems[last];
	return _lexems[pos];
}

SyntaxToken Parser::current_token()
{
	return _lexems[_current];
}

SyntaxToken Parser::next_token()
{
	return _lexems[++_current];
}

SyntaxToken Parser::lookahead()
{
	int ahead = _current + 1;
	return _lexems[ahead];
}


std::vector<AstNode*>* Parser::parse()
{
	std::vector<AstNode*>* ast_trees = new std::vector<AstNode*>;
	for (int i = 0; i < _lines; i++)
	{
		ast_trees->push_back(new AstNode(AstTag::STMT));
		stmt(ast_trees->at(i));
	}
	return ast_trees;
}


std::vector<SyntaxToken>* Parser::get_lexems()
{
	return &_lexems;
}


bool Parser::stmt(AstNode* stmt_node)
{
	// STMT -> ID := EXPR;
	if (current_token().token_type == SyntaxTag::ID_TOKEN)
	{
		stmt_node->add_child(new SyntaxToken(current_token()));
		if (next_token().token_type == SyntaxTag::ASSIGN_TOKEN)
		{
			stmt_node->add_child(new SyntaxToken(current_token()));
			AstNode* expr_node = new AstNode(AstTag::EXPRESSION);
			if (expr(expr_node))
			{
				stmt_node->add_child(expr_node);
				if (next_token().token_type == SyntaxTag::SEMICOLON_TOKEN)
				{
					stmt_node->add_child(new SyntaxToken(current_token()));
					next_token();
					return true;
				}
			}
		}
		else
			std::cout << " ERROR -> unexpected token " << std::endl;
	}
	else
	{
		// to do: add list of errors for printing after parsing
		std::cout << "ERROR -> unexpected token: expected - ID_TOKEN, got - " << std::endl;
	}
	return false;
}

bool Parser::expr(AstNode* expr_node)
{
	// EXPR -> TRANS ADD_SUB
	AstNode* trans_node = new AstNode(AstTag::TRANS);
	if (trans(trans_node))
	{
		expr_node->add_child(trans_node);
		AstNode* add_sub_node = new AstNode(AstTag::ADD_SUB);
		if (add_sub(add_sub_node))
			expr_node->add_child(add_sub_node);
		return true;
	}
	return false;
}

bool Parser::add_sub(AstNode* add_sub_node)
{
	// ADD_SUB -> + TRANS ADD_SUB
	SyntaxToken word = lookahead();
	if (word.token_type == SyntaxTag::PLUS_TOKEN || word.token_type == SyntaxTag::MINUS_TOKEN)
	{
		add_sub_node->add_child(new SyntaxToken(current_token()));
		next_token();
		AstNode* trans_node = new AstNode(AstTag::TRANS);
		if (trans(trans_node))
		{
			add_sub_node->add_child(trans_node);
			AstNode* add_sub_node_nested = new AstNode(AstTag::ADD_SUB);
			if (add_sub(add_sub_node_nested))
				add_sub_node->add_child(add_sub_node_nested);
			return true;
		}
	}
	return false;
}

bool Parser::trans(AstNode* trans_node)
{
	// TRANS -> FACTOR MUL_DIV
	AstNode* factor_node = new AstNode(AstTag::FACTOR);
	if (factor(factor_node))
	{
		trans_node->add_child(factor_node);
		AstNode* mul_div_node = new AstNode(AstTag::MUL_DIV);
		if (mul_div(mul_div_node))
			trans_node->add_child(mul_div_node);
		return true;
	}
	return false;
}

bool Parser::mul_div(AstNode* mul_div_node)
{
	// MUL_DIV -> FACTOR MUL_DIV
	SyntaxToken word = lookahead();
	if (word.token_type == SyntaxTag::STAR_TOKEN || word.token_type == SyntaxTag::SLASH_TOKEN)
	{
		mul_div_node->add_child(new SyntaxToken(current_token()));
		next_token();
		AstNode* factor_node = new AstNode(AstTag::FACTOR);
		if (factor(factor_node))
		{
			mul_div_node->add_child(factor_node);
			AstNode* mul_div_node_nested = new AstNode(AstTag::MUL_DIV);
			if (mul_div(mul_div_node_nested))
				mul_div_node->add_child(mul_div_node_nested);
			return true;
		}
	}
	return false;
}

bool Parser::factor(AstNode* factor_node)
{
	// FACTOR -> ( EXPR ) | FLOAT_NUM | ID_TOKEN
	if (next_token().token_type == SyntaxTag::LP_TOKEN)
	{
		factor_node->add_child(new SyntaxToken(current_token()));
		AstNode* expr_node = new AstNode(AstTag::EXPRESSION);
		if (expr(expr_node))
		{
			factor_node->add_child(expr_node);
			if (next_token().token_type == SyntaxTag::RP_TOKEN)
			{
				factor_node->add_child(new SyntaxToken(current_token()));
				return true;
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
		factor_node->add_child(new SyntaxToken(current_token()));
		return true;
	}
	return false;
}

