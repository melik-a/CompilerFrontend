#include "../include/SyntaxTag.h"

std::ostream& operator<<(std::ostream& os, const SyntaxTag& tag)
{
	switch (tag)
	{
		case SyntaxTag::ID_TOKEN:
			os << "ID_TOKEN";
			break;
		case SyntaxTag::DOUBLE_NUMBER:
			os << "DOUBLE_NUMBER";
			break;
		case SyntaxTag::INTEGER_NUMBER:
			os << "INTEGER_NUMBER";
			break;
		case SyntaxTag::CHAR_LITERAL:
			os << "CHAR_LITERAL";
			break;
		case SyntaxTag::BOOL_TYPE:
			os << "BOOL_TYPE";
			break;
		case SyntaxTag::FLOAT_NUMBER:
			os << "FLOAT_NUMBER";
			break;
		case SyntaxTag::ARRAY_TOKEN:
			os << "ARRAY_TOKEN";
			break;
		case SyntaxTag::BEGIN_TOKEN:
			os << "BEGIN_TOKEN";
			break;
		case SyntaxTag::BYTE_TYPE:
			os << "BYTE_TYPE";
			break;
		case SyntaxTag::CHAR_TYPE:
			os << "CHAR_TYPE";
			break;
		case SyntaxTag::COLON_TOKEN:
			os << "COLON_TOKEN";
			break;
		case SyntaxTag::COMMA_TOKEN:
			os << "COMMA_TOKEN";
			break;
		case SyntaxTag::DOT_TOKEN:
			os << "DOT_TOKEN";
			break;
		case SyntaxTag::DOUBLE_TYPE:
			os << "DOUBLE_TYPE";
			break;
		case SyntaxTag::FALSE:
			os << "FALSE";
			break;
		case SyntaxTag::TRUE:
			os << "TRUE";
			break;
		case SyntaxTag::FLOAT_TYPE:
			os << "FLOAT_TYPE";
			break;
		case SyntaxTag::INTEGER_TYPE:
			os << "INTEGER_TYPE";
			break;
		case SyntaxTag::LSB_TOKEN:
			os << "LSB_TOKEN";
			break;
		case SyntaxTag::RSB_TOKEN:
			os << "RSB_TOKEN";
			break;
		case SyntaxTag::OF_TOKEN:
			os << "OF_TOKEN";
			break;
		case SyntaxTag::PROGRAM_TOKEN:
			os << "PROGRAM_TOKEN";
			break;
		case SyntaxTag::VAR_TOKEN:
			os << "VAR_TOKEN";
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