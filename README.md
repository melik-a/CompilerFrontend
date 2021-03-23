# CompilerFrontend
Simple compiler frontend part for pseudo-pascal simple statement.
Front consist of: simple lexer based on state machine, simple symbol table based on hash table, simple LL(1) parser based on recursive descent.

# General
At this moment front implements simple pseudo-pascal program grammar that consists of variable declarations and assignment statements.
A little bit notations:

	GLOBAL - GLOBAL SCOPE
	PROGRAM - PROGRAM STRUCTURE DESCRIPTION
	PROG_DEF - PROGRAM DEFINITION - JUST KEYWORD AND PROGRAM NAME
	STMT - STATEMENT
	VAR_DECL - VARIABLE DECLARATION BLOCK
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
	
		GLOBAL		->		PROGRAM
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

	parser type LL(1)
	algorithm - recursive descent


example of syntactically correct program:
	
	program my_first_program;
	var 
		some_var, another_one, _ten, result: float;
		int_variable : integer;
	begin
		some_var := 0.9 * 10.5;
		another_one := some_var + 3.0;
		_ten := 10e-2;
		int_variable := 20;
		{ some comment here}
		result {another comment} := (some_var *another_one) / (some_var - _ten) * _ten;
	end.

program parsing example:
	
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
	
statement parsing expamples:
	
	source code line - some_var := 0.9 * 10.5;
	lexical analyzer output - 
		<ID, some_var> <ASSIGN, := > <FLOAT_NUMBER, 0.9> <ARITHM_OPERATOR, * > <FLOAT_NUMBER, 10.5>
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

# Front parts description

Symbol table:

    is a simple implementation of hash table thats supports adding identificators.
    hash table template arguments: K - type of storage key,
                        V - type of storage value,
                        H - hash function (by dedault used Hash<K>{})

    implementation provides some general methods: insert, remove, search, upgrade, resize and etc.

    to use this hash table implementaion with some non-trivial structures
    need to transfer as a template argument some hash functions that can provide hashing this structure.

    also hash table supports move semantics.

Lexer:
  
    is a simple state machine which reads source code lines from the file 
    and recognize syntax elements (SyntaxToken).
    the lexer builds a table of tokens.

Parser:
    
    the parser checks the syntactic correctness of the sequence of tokens, 
    adds identifiers to the identifier table, and builds an abstract syntax tree.
    the parser type is LL(1), algorithm that is used - recursive descent.
    An abstract syntax tree is just a set of nodes (AstNode), 
    each node stores information in itself, and can also have child nodes.

___melik-a,___ 
___2021___ 
