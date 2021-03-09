#pragma once

#include <vector>
#include <fstream>

#include "SyntaxToken.h"
#include "AstNode.h"
#include "../../SymbolTable/HashMap/HashMap.h"


/*	STMT - START POINT
	ID - IDENTIFICATOR
	ASSIGN - ASSIGN OPERATOR - :=
	EXPR - COMMON EXPRESSION
	TRANS - TRANSIT EXPRESSION
	ADD_SUB - ADDITION AND SUBTRACTION RIGH-RECURSIVE EXPRESSION
	MUL_DIV - MULTIPLICATION AND DIVISION RIGH-RECURSIVE EXPRESSION
	FACTOR - DEVIDED NON-TERMINAL
	EPS - EPSILON (EMPTY SET)

	
	grammar rules:
	
		STMT	->	ID := EXPR;
		EXPR	->	TRANS ADD_SUB
		ADD_SUB ->	+ TRANS ADD_SUB
				 |	- TRANS ADD_SUB
				 |	EPS
		TRANS	->	FACTOR MUL_DIV
		MUL_DIV ->	* FACTOR MUL_DIV
				 |	/ FACTOR MUL_DIV
				 |	EPS
		FACTOR	->	( EXPR )
				 |	FLOAT_NUM
				 |	ID

	parser type LL(1)
	algorithm - recursive descent

*/


/*	example of syntactically correct sentences:
	
	some_var := 0.9 * 10.5;
	another_one := some_var + 3.0;
	_ten := 10e-2;
	{ fafasfag asfasfa}
	result {fafasfag asfasfa} := (some_var *another_one) / (some_var - _ten) * _ten;
	

*/

/*	parsing expamples:
	
	source code line - some_var := 0.9 * 10.5;
	lexical analyzer output - <ID, some_var> <ASSIGN, := > <FLOAT_NUMBER, 0.9> <ARITHM_OPERATOR, * > <FLOAT_NUMBER, 10.5>
	syntax analyzer output -				
	|___ <STMT>
		|--- <ID_TOKEN, some_var>
		|--- <ASSIGN_TOKEN, :=>
		|--- <EXPRESSION>
		|    |___ <TRANS>
		|        |--- <FACTOR>
		|        |    |___ <FLOAT_NUMBER, 0.9>
		|        |___ <MUL_DIV>
		|            |--- <FLOAT_NUMBER, 0.9>
		|            |___ <FACTOR>
		|                |___ <FLOAT_NUMBER, 10.5>
		|___ <SEMICOLON_TOKEN, ;>							
*/


struct Parser
{
	Parser(std::vector<SyntaxToken>* lex_table, size_t lines) : _lexems(*lex_table), _lines(lines) {}

	std::vector<AstNode*>* parse(HashMap<std::string, float>& symbol_table);

	std::vector<SyntaxToken>* get_lexems();

	private:
		std::vector<SyntaxToken>& _lexems;

		int _current{};
		int _lines{};

		SyntaxToken peek_token(size_t pos) const;
		SyntaxToken current_token() const;
		SyntaxToken next_token();
		SyntaxToken lookahead() const;
		
		bool stmt(AstNode* stmt_node, HashMap<std::string, float>& symbol_table);
		bool expr(AstNode* expr_node, HashMap<std::string, float>& symbol_table);
		bool add_sub(AstNode* add_sub_node, HashMap<std::string, float>& symbol_table);
		bool trans(AstNode* trans_node, HashMap<std::string, float>& symbol_table);
		bool mul_div(AstNode* mul_div_node, HashMap<std::string, float>& symbol_table);
		bool factor(AstNode* factor_node, HashMap<std::string, float>& symbol_table);
};
