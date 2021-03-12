#pragma once

#include <vector>
#include <fstream>

#include "SyntaxToken.h"
#include "AstNode.h"
#include "../../SymbolTable/HashMap/HashMap.h"
#include "../../Errors/Error.h"


/*	STMT - STATEMENT - START POINT FOR A WHILE
	VAR - VARIABLE
	DECL - DECLARATION
	D_LIST - LIST OF DECLARATIONS - COMMA SEPARATED DECLARATIONS
	ID - IDENTIFICATOR
	ASSIGN - ASSIGN OPERATOR - :=
	EXPR - COMMON EXPRESSION
	TRANS - TRANSIT EXPRESSION
	ADD_SUB - ADDITION AND SUBTRACTION RIGH-RECURSIVE EXPRESSION
	MUL_DIV - MULTIPLICATION AND DIVISION RIGH-RECURSIVE EXPRESSION
	FACTOR - DEVIDED NON-TERMINAL
	EPS - EPSILON (EMPTY SET)

	
	grammar rules:

		STMT		->		VAR DECL;	|	ID := EXPR;
		DECL		->		ID ID_LIST	:	TYPE D_LIST
		D_LIST		->		; DECL 
					|		EPS
		ID_LIST		->		, ID_LIST
					|		EPS
		TYPE		->		array [ ARR_SIZE ] of TYPE
					|		BASIC_TYPE
		EXPR		->		TRANS ADD_SUB
		ADD_SUB		->		+ TRANS ADD_SUB
					|		- TRANS ADD_SUB
					|		EPS
		TRANS		->		FACTOR MUL_DIV
		MUL_DIV		->		* FACTOR MUL_DIV
					|		/ FACTOR MUL_DIV
					|		EPS
		FACTOR		->		( EXPR )
					|		BASIC_TYPE
					|		ID
		BASIC_TYPE	->		FLOAT | INTEGER | BOOLEAN | DOUBLE | CHAR | BYTE

	parser type LL(1)
	algorithm - recursive descent

*/


/*	example of syntactically correct sentences:
	
	var some_var, another_one, _ten, result: float;
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
		|            |--- <STAR_TOKEN, *>
		|            |___ <FACTOR>
		|                |___ <FLOAT_NUMBER, 10.5>
		|___ <SEMICOLON_TOKEN, ;>							
*/


struct Parser
{
	using hash_map = HashMap<std::string, float>;

	Parser(std::vector<SyntaxToken>* lex_table, size_t lines) : _lexems(*lex_table), _lines(lines) {}

	std::vector<AstNode*>* parse(hash_map& symbol_table, std::vector<Error>& error_list);

	std::vector<SyntaxToken>* get_lexems();

	private:
		std::vector<SyntaxToken>& _lexems;

		size_t _current{};
		size_t _lines{};
		static const unsigned int _error_level = 0x0000A001;

		SyntaxToken peek_token(size_t pos) const;
		SyntaxToken current_token() const;
		SyntaxToken next_token();
		SyntaxToken lookahead() const;
		
		bool stmt(AstNode* stmt_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool expr(AstNode* expr_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool add_sub(AstNode* add_sub_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool trans(AstNode* trans_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool mul_div(AstNode* mul_div_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool factor(AstNode* factor_node, hash_map& symbol_table, std::vector<Error>& error_list);
};
