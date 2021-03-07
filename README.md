# CompilerFrontend
Simple compiler frontend part for pseudo-pascal simple statement.
Front consist of: simple lexer based on state machine, simple symbol table based on hash table, simple LL(1) parser based on recursive descent.

# General
At this moment front implements simple pseudo-pascal statements grammar of assignment expressions.
A little bit notations:

    STMT - START POINT
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


example of syntactically correct sentences:
	
	some_var := 0.9 * 10.5;
	another_one := some_var + 3.0;
	_ten := 10e-2;
	{ fafasfag asfasfa}
	result {fafasfag asfasfa} := (some_var *another_one) / (some_var - _ten) * _ten;
	
parsing expamples:
	
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
  
    is a simple state machine which reads source code lines from the file and recognize syntax elements (SyntaxToken).
    the lexer builds a table of tokens.

Parser:
    
    the parser checks the syntactic correctness of the sequence of tokens, 
    adds identifiers to the identifier table, and builds an abstract syntax tree.
    the parser type is LL(1), algorithm that is used - recursive descent.
    An abstract syntax tree is just a set of nodes (AstNode), 
    each node stores information in itself, and can also have child nodes.

