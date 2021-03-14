#include "Scanner.h"


LexicalScanner::LexicalScanner(std::string& file)
{
	_file_name = file;
	_file.open(_file_name);
	if (!_file)
		throw std::runtime_error("invalid file name or path");
	_lexeme_table = new std::vector<SyntaxToken>();
	_keywords = { 
		{"array",SyntaxTag::ARRAY_TOKEN},
		{"begin", SyntaxTag::BEGIN_TOKEN},
		{"bool", SyntaxTag::BOOL_TYPE},
		{"byte", SyntaxTag::BYTE_TYPE},
		{"char", SyntaxTag::CHAR_TYPE},
		{"double", SyntaxTag::DOUBLE_TYPE},
		{"end", SyntaxTag::END_TOKEN},
		{"float", SyntaxTag::FLOAT_TYPE},
		{"false", SyntaxTag::FALSE},
		{"integer", SyntaxTag::INTEGER_TYPE},
		{"of", SyntaxTag::OF_TOKEN},
		{"program", SyntaxTag::PROGRAM_TOKEN},
		{"true", SyntaxTag::TRUE},
		{"var", SyntaxTag::VAR_TOKEN}
	};
}


LexicalScanner::~LexicalScanner()
{
	if (_file)
		_file.close();
}


std::vector<SyntaxToken>* LexicalScanner::scan(std::vector<Error>& error_list)
{
	_lexeme_table->clear();
	while (std::getline(_file, _current_line, '\n'))
	{
		for (_current_pos = 0; _current_pos < _current_line.length();)
		{
			switch (_current_line[_current_pos])
			{
				case '\'':
					char_literal_lexeme_scan(error_list);
					break;
				// key one character lexemes
				case '(':
					_lexeme_table->push_back(SyntaxToken{ "(", SyntaxTag::LP_TOKEN,
														_line_counter, _current_pos });
					_current_pos++;
					break;
				case ')':
					_lexeme_table->push_back(SyntaxToken{ ")", SyntaxTag::RP_TOKEN,
														_line_counter, _current_pos });
					_current_pos++;
					break;
				case '[':
					_lexeme_table->push_back(SyntaxToken{ "[", SyntaxTag::LSB_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case ']':
					_lexeme_table->push_back(SyntaxToken{ "]", SyntaxTag::RSB_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case ';':
					_lexeme_table->push_back(SyntaxToken{ ";", SyntaxTag::SEMICOLON_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case ':':
					assign_or_colon_scan();
					break;
				case ',':
					_lexeme_table->push_back(SyntaxToken{ ",", SyntaxTag::COMMA_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case '.':
					_lexeme_table->push_back(SyntaxToken{ ".", SyntaxTag::DOT_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case '+':
					_lexeme_table->push_back(SyntaxToken{ "+", SyntaxTag::PLUS_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case '-':
					_lexeme_table->push_back(SyntaxToken{ "-", SyntaxTag::MINUS_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case '/':
					_lexeme_table->push_back(SyntaxToken{ "/", SyntaxTag::SLASH_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case '*':
					_lexeme_table->push_back(SyntaxToken{ "*", SyntaxTag::STAR_TOKEN,
										_line_counter, _current_pos });
					_current_pos++;
					break;
				case '{':
					ignore_comment(error_list);
					break;
				case ' ':
					_current_pos++;
					break;
				case '\t':
					_current_pos++;
					break;
				default:
					if (is_letter(_current_line[_current_pos]))
					{
						id_lexeme_scan();
					}
					else if (is_digit(_current_line[_current_pos])) // scan number literals
					{
						number_lexeme_scan();
					}
					else
					{
						std::string error_char{};
						error_char = _current_line[_current_pos];
						error_list.push_back(Error(_file_name, _error_level, ErrorTag::LEXICAL_ERROR,
							"lexer can't recognize character as an input language character. Bad character \"" + error_char + '\"', _line_counter, _current_pos));
						_current_pos++;
					}
					break;
			}
		}
		_line_counter++;
	}
	if (_file.is_open())
		_file.close();
	_lexeme_table->push_back(SyntaxToken{ "", SyntaxTag::END_OF_FILE,
										_line_counter, _current_pos });
	return _lexeme_table;
}


void LexicalScanner::id_lexeme_scan()
{
	std::string lexeme = "";
	char symbol = _current_line[_current_pos];
	bool not_terminal{ true };
	while (not_terminal && (is_digit(symbol) || is_letter(symbol)))
	{
		lexeme += symbol;
		symbol = _current_line[++_current_pos];
		not_terminal = _current_pos < _current_line.length();
	}
	auto word = _keywords.find(lexeme);
	if (word != _keywords.end())
	{
		_lexeme_table->push_back(SyntaxToken{ word->first, word->second, _line_counter, _current_pos });
	}
	else
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN,
						_line_counter, _current_pos });
	}
}


void LexicalScanner::number_lexeme_scan()
{
	std::string lexeme = "";
	char symbol = _current_line[_current_pos];
	bool not_terminal{ true };
	while (not_terminal && is_digit(symbol))
	{
		lexeme += symbol;
		symbol = _current_line[++_current_pos];
		not_terminal = _current_pos < _current_line.length();
	}

	if (not_terminal && (symbol == '.' || symbol == 'e')) // floating point number
	{
		do 
		{
			lexeme += symbol;
			symbol = _current_line[++_current_pos];
			not_terminal = _current_pos < _current_line.length();
		}
		while (is_digit(symbol) || symbol == '.' || symbol == 'e' || symbol == '+' || symbol == '-');
		if (symbol == 'f')
		{
			_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER,
									_line_counter, _current_pos });
		}
		else
		{
			_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::DOUBLE_NUMBER,
									_line_counter, _current_pos }); 
		}
	}
	else // integer number
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::INTEGER_NUMBER,
										_line_counter, _current_pos });
	}	
}


void LexicalScanner::assign_or_colon_scan()
{
	std::string lexeme{};
	lexeme += _current_line[_current_pos];
	char symbol = _current_line[++_current_pos];
	bool not_terminal = _current_pos < _current_line.length();

	if (not_terminal && symbol == '=')
	{
		lexeme += symbol;
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ASSIGN_TOKEN,
											_line_counter, _current_pos });
	}
	else
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::COLON_TOKEN,
										_line_counter, _current_pos });
	}
	_current_pos++;
}


void LexicalScanner::ignore_comment(std::vector<Error>& error_list)
{
	char symbol = _current_line[_current_pos];
	bool not_terminal{ true };
	while (not_terminal && symbol != '}')
	{
		if (symbol == '\n')
			_line_counter++;
		symbol = _current_line[++_current_pos];
		not_terminal = _current_pos < _current_line.length();
	}
	if (!not_terminal)
	{
		error_list.push_back(Error(_file_name, _error_level, ErrorTag::LEXICAL_ERROR,
			"not closed comment.", _line_counter, _current_pos));
	}
	else
		++_current_pos;
}


void LexicalScanner::char_literal_lexeme_scan(std::vector<Error>& error_list)
{
	std::string lexeme = "";
	char symbol = _current_line[_current_pos];
	lexeme += symbol;
	
	lexeme += symbol;
	symbol = _current_line[++_current_pos];
	if (symbol == '\'')
	{
		lexeme += symbol;
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::CHAR_LITERAL,
									_line_counter, _current_pos });
	}
	else
	{
		error_list.push_back(Error(_file_name, _error_level, ErrorTag::LEXICAL_ERROR,
			"lexer can't identify character literal lexeme, not closed single quote.", _line_counter, _current_pos));
	}
	++_current_pos;
}


bool LexicalScanner::is_digit(char symbol) const
{
	if (symbol >= '0' && symbol <= '9')
		return true;
	return false;
}


bool LexicalScanner::is_letter(char symbol) const
{
	if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z') || symbol == '_')
		return true;
	return false;
}


size_t LexicalScanner::get_num_of_lines()
{
	return _line_counter;
}

