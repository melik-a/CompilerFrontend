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
	ID_LIST - LIST OF IDENTIFIER
	ID - IDENTIFIER
	TYPE - DATA TYPE OF IDENTIFIER
	BASIC_TYPE - SOME BASIC DATA TYPES
	ASSIGN - ASSIGN OPERATOR - :=
	EXPR - COMMON EXPRESSION
	TRANS - TRANSIT EXPRESSION
	ADD_SUB - ADDITION AND SUBTRACTION RIGH-RECURSIVE EXPRESSION
	MUL_DIV - MULTIPLICATION AND DIVISION RIGH-RECURSIVE EXPRESSION
	FACTOR - DEVIDED NON-TERMINAL
	EPS - EPSILON (EMPTY SET)

	
	grammar rules:

		GLOB		->		PROGRAM
		PROGRAM		->		PROG_DEF VAR_DECL BLOCK
		PROG_DEF	->		PROGRAM ID;
		VAR_DECL	->		VAR DECL
		DECL		->		ID_LIST	:	TYPE ;
					|		EPS
		ID_LIST		->		ID, ID_LIST
					|		EPS
		TYPE		->		array [ ARR_SIZE ] of TYPE
					|		BASIC_TYPE
		ARR_SIZE	->		1..BYTE_NUMBER
		BLOCK		->		BEGIN STMT END.
		STMT		->		ID := EXPR;
		EXPR		->		TRANS ADD_SUB
		ADD_SUB		->		+ TRANS ADD_SUB
					|		- TRANS ADD_SUB
					|		EPS
		TRANS		->		FACTOR MUL_DIV
		MUL_DIV		->		* FACTOR MUL_DIV
					|		/ FACTOR MUL_DIV
					|		EPS
		FACTOR		->		( EXPR )
					|		FLOAT_NUMBER_NUMBER | INTEGER_NUMBER | BYTE_NUMBER | DOUBLE_NUMBER | CHAR_LITERAL | FALSE | TRUE 
					|		ID
		BASIC_TYPE	->		FLOAT_NUMBER_TYPE | INTEGER_TYPE | BOOL_TYPE | DOUBLE_TYPE | CHAR_TYPE | BYTE_TYPE		



		{unary idea}
		TRANS		->		UNARY MUL_DIV
		MUL_DIV		->		* UNARY MUL_DIV
					|		/ UNARY MUL_DIV
					|		EPS
		UNARY		->		FACTOR
					|		- FACTOR
					|		! FACTOR
	parser type LL(1)
	algorithm - recursive descent

*/


/*	example of syntactically correct program:
	
	program my_first_program;
	var 
		some_var, another_one, _ten, result: FLOAT_NUMBER;
		int_variable : integer;
	begin
		some_var := 0.9 * 10.5;
		another_one := some_var + 3.0;
		_ten := 10e-2;
		int_variable := 20;
		{ some comment here}
		result {another comment} := (some_var *another_one) / (some_var - _ten) * _ten;
	end.

|___ <GLOBAL>
    |___ <PROGRAM>
        |--- <PROG_DEF>
        |    |--- <PROGRAM_TOKEN, program>
        |    |--- <ID_TOKEN, my_first_program>
        |    |___ <SEMICOLON_TOKEN, ;>
        |--- <VAR_DECL>
        |    |--- <VAR_TOKEN, var>
        |    |--- <DECL>
        |    |    |--- <ID_LIST>
        |    |    |    |--- <ID_TOKEN, some_var>
        |    |    |    |--- <COMMA_TOKEN, ,>
        |    |    |    |--- <ID_TOKEN, another_one>
        |    |    |    |--- <COMMA_TOKEN, ,>
        |    |    |    |--- <ID_TOKEN, _ten>
        |    |    |    |--- <COMMA_TOKEN, ,>
        |    |    |    |___ <ID_TOKEN, result>
        |    |    |--- <COLON_TOKEN, :>
        |    |    |--- <TYPE>
        |    |    |    |___ <BASIC_TYPE>
        |    |    |        |___ <FLOAT_TYPE, float>
        |    |    |___ <SEMICOLON_TOKEN, ;>
        |    |___ <DECL>
        |        |--- <ID_LIST>
        |        |    |___ <ID_TOKEN, int_variable>
        |        |--- <COLON_TOKEN, :>
        |        |--- <TYPE>
        |        |    |___ <BASIC_TYPE>
        |        |        |___ <INTEGER_TYPE, integer>
        |        |___ <SEMICOLON_TOKEN, ;>
        |___ <BLOCK>
            |--- <BEGIN_TOKEN, begin>
            |--- <STMT>
            |    |--- <ID_TOKEN, some_var>
		... ... ... ... ... ... ... ...

*/	

/*	statement parsing expamples:
	
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
		
		bool global(AstNode* global_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool program(AstNode* program_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool prog_def(AstNode* prog_def_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool var_decl(AstNode* var_decl_node, hash_map& symbol_table, std::vector<Error>& error_list);

		bool decl(AstNode* decl_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool id_list(AstNode* id_list_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool type(AstNode* type_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool arr_size(AstNode* arr_size_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool basic_type(AstNode* basic_type_node, hash_map& symbol_table, std::vector<Error>& error_list);		

		bool block(AstNode* block_node, hash_map& symbol_table, std::vector<Error>& error_list);

		bool stmt(AstNode* stmt_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool expr(AstNode* expr_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool add_sub(AstNode* add_sub_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool trans(AstNode* trans_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool mul_div(AstNode* mul_div_node, hash_map& symbol_table, std::vector<Error>& error_list);
		bool factor(AstNode* factor_node, hash_map& symbol_table, std::vector<Error>& error_list);
};
