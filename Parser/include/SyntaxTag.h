#pragma once
#include <iostream>
enum class SyntaxTag
{
	PROGRAM,			// program keyword


	ID_TOKEN,			// { a-z || A-Z || _ }, {a-z || A-Z || _ || 0-9 }

	FLOAT_TYPE,			// float
	INTEGER_TYPE,		// integer
	BYTE_TYPE,			// byte
	BOOL_TYPE,			// bool
	DOUBLE_TYPE,		// double
	CHAR_TYPE,			// char

	FLOAT_NUMBER,		// 
	INTEGER_NUMBER,		// 
	DOUBLE_NUMBER,		// 
	BYTE_NUMBER,		// 

	CHAR_LITERAL,		// symbol in single quotes

	TRUE,				//
	FALSE,				// 

	ASSIGN_TOKEN,		// :=

	PLUS_TOKEN,			// +
	MINUS_TOKEN,		// -
	STAR_TOKEN,			// *
	SLASH_TOKEN,		// /

	LSB_TOKEN,			// [ - LEFT SQUARE BRACKET
	RSB_TOKEN,			// ] - RIGHT SQUARE BRACKET 
	LP_TOKEN,			// ( - LEFT PARENTHESIS
	RP_TOKEN,			// ) -RIGHT PARENTHESIS
	SEMICOLON_TOKEN,	// ;
	COMMA_TOKEN,		// ,
	VAR_TOKEN,			// VAR
	COLON_TOKEN,		// :
	DOT_TOKEN,			// .
	
	ARRAY_TOKEN,		// array - array [1..20] of
	OF_TOKEN,			// of - - array [1..20] of
	
	END_OF_FILE,
	UNKNOWN_TOKEN
};

std::ostream& operator<<(std::ostream& os, const SyntaxTag& tag);