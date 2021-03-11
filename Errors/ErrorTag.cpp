#include "ErrorTag.h"

std::ostream& operator << (std::ostream& os, const ErrorTag& tag)
{
	switch (tag)
	{
		case ErrorTag::SYNTAX_ERROR:
			os << "Syntax Error";
			break;
		case ErrorTag::LEXICAL_ERROR:
			os << "Lexical Error";
			break;
		case ErrorTag::WARNING:
			os << "Warning";
			break;
		default:
			os << "Error";
			break;
	}
	return os;
}