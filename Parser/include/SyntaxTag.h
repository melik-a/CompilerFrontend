#pragma once
#include <iostream>
enum class SyntaxTag
{
	ID_TOKEN,
	FLOAT_NUMBER,
	
	ASSIGN_TOKEN,
	
	PLUS_TOKEN,
	MINUS_TOKEN,
	STAR_TOKEN,
	SLASH_TOKEN,
		
	LP_TOKEN,
	RP_TOKEN,
	SEMICOLON_TOKEN,
	
	END_OF_FILE,
	UNKNOWN_TOKEN
};

std::ostream& operator<<(std::ostream& os, const SyntaxTag& tag);