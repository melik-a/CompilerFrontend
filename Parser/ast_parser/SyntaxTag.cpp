#include "../include/SyntaxTag.h"

std::ostream& operator<<(std::ostream& os, const SyntaxTag& tag)
{
	switch (tag)
	{
		case SyntaxTag::ID_TOKEN:
			os << "ID_TOKEN";
			break;
		case SyntaxTag::FLOAT_NUMBER:
			os << "FLOAT_NUMBER";
			break;
		case SyntaxTag::ASSIGN_TOKEN:
			os << "ASSIGN_TOKEN";
			break;
		case SyntaxTag::PLUS_TOKEN:
			os << "PLUS_TOKEN";
			break;
		case SyntaxTag::MINUS_TOKEN:
			os << "MINUS_TOKEN";
			break;
		case SyntaxTag::STAR_TOKEN:
			os << "STAR_TOKEN";
			break;
		case SyntaxTag::SLASH_TOKEN:
			os << "SLASH_TOKEN";
			break;
		case SyntaxTag::LP_TOKEN:
			os << "LP_TOKEN";
			break;
		case SyntaxTag::RP_TOKEN:
			os << "RP_TOKEN";
			break;
		case SyntaxTag::SEMICOLON_TOKEN:
			os << "SEMICOLON_TOKEN";
			break;
		case SyntaxTag::END_OF_FILE:
			os << "END_OF_FILE";
			break;
		case SyntaxTag::UNKNOWN_TOKEN:
			os << "UNKNOWN_TOKEN";
			break;
		default:
			os << "UNKNOWN_TOKEN";
	}
	return os;
}