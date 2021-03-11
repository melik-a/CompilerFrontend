#pragma once
#include <iostream>

enum class ErrorTag : _int16
{
	SYNTAX_ERROR,
	LEXICAL_ERROR,
	WARNING
};

std::ostream& operator << (std::ostream& os, const ErrorTag& tag);