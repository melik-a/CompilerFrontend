#include "Scanner.h"


LexicalScanner::LexicalScanner(std::string& file)
{
	_file.open(file);
	if (!_file)
	{
		std::cout << "\nsomething went wrong. invalid file name or path" << std::endl;
		throw std::runtime_error("invalid file name or path");
	}
	_current_state = States::START;
	_lexeme_table = new std::vector<SyntaxToken>();
}


LexicalScanner::~LexicalScanner()
{
	if (_file)
		_file.close();
}


std::vector<SyntaxToken>* LexicalScanner::scan(std::vector<Error>& error_list)
{
	_lexeme_table->clear();
	std::string lexeme = "";

	char curr_symbol = ' ';

	while (!_file.eof())
	{
		curr_symbol = _file.get();
		switch (_current_state)
		{
			case States::START:
				start_state_changing(curr_symbol, lexeme);
				break;
			case States::ID:
				id_state_changing(curr_symbol, lexeme);
				break;
			case States::ARITHM_OPERATOR:
				arithm_operator_state_changing(curr_symbol, lexeme);
				break;
			case States::FLOAT_NUMBER:
				float_num_state_changing(curr_symbol, lexeme);
				break;
			case States::ASSIGNMENT:
				assignment_state_changing(curr_symbol, lexeme);
				break;
			case States::SEPARATOR:
				separator_state_changing(curr_symbol, lexeme);
				break;
			case States::COMMENT:
				comment_state_changing(curr_symbol, lexeme);
				break;
			case States::ERROR:
				error_state_changing(curr_symbol, lexeme, error_list);
				break;
		}
	}
	if (_file.is_open())
		_file.close();
	_lexeme_table->push_back(SyntaxToken{ "", SyntaxTag::END_OF_FILE, 
										_line_counter, _symbol_pos_at_line });
	return _lexeme_table;
}


size_t LexicalScanner::get_num_of_lines()
{
	return _line_counter;
}


size_t LexicalScanner::get_num_of_codelines()
{
	return _code_line_counter;
}


void LexicalScanner::start_state_changing(char symbol, std::string& lexeme)
{
	if (is_letter(symbol))
	{
		lexeme += symbol;
		_current_state = States::ID;
		_symbol_pos_at_line++;
	}
	else if (is_digit(symbol))
	{
		lexeme += symbol;
		_current_state = States::FLOAT_NUMBER;
		_symbol_pos_at_line++;
	}
	else if (symbol == ';' || symbol == '(' || symbol == ')')
	{
		lexeme += symbol;
		_current_state = States::SEPARATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		lexeme += symbol;
		_current_state = States::ARITHM_OPERATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == ':')
	{
		lexeme += symbol;
		_current_state = States::ASSIGNMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == '{')
	{
		_current_state = States::COMMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == '\n' || symbol == '\t' || symbol == ' ')
	{
		if (symbol == '\n')
		{
			_line_counter++;
			_symbol_pos_at_line = 0;
		}
		else
			_symbol_pos_at_line++;
		return;
	}
	else
	{
		lexeme += symbol;
		_current_state = States::ERROR;
		_symbol_pos_at_line++;
	}
}


void LexicalScanner::id_state_changing(char symbol, std::string& lexeme)
{
	if (is_digit(symbol) || is_letter(symbol))
	{
		lexeme += symbol;
		_symbol_pos_at_line++;
	}
	else if (symbol == ':')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN,
											_line_counter, _symbol_pos_at_line });
		lexeme = symbol;
		_current_state = States::ASSIGNMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN,
											_line_counter, _symbol_pos_at_line });
		lexeme = symbol;
		_current_state = States::ARITHM_OPERATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == '{')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN,
											_line_counter, _symbol_pos_at_line });
		lexeme = "";
		_current_state = States::COMMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == ';' || symbol == ')')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN,
											_line_counter, _symbol_pos_at_line });
		lexeme = symbol;
		_current_state = States::SEPARATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ID_TOKEN,
											_line_counter, _symbol_pos_at_line });
		lexeme = "";
		_current_state = States::START;
		if (symbol == '\n')
		{
			_line_counter++;
			_symbol_pos_at_line = 0;
		}
		else
			_symbol_pos_at_line++;
	}
	else
	{
		lexeme = "error on ID state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
		_symbol_pos_at_line++;
	}
}


void LexicalScanner::assignment_state_changing(char symbol, std::string& lexeme)
{
	if (symbol == '=')
	{
		lexeme += symbol;
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::ASSIGN_TOKEN,
											_line_counter, _symbol_pos_at_line });
		lexeme = "";
		_current_state = States::START;
	}
	else
	{
		lexeme = "error on ASSIGNMENT state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
	}
	_symbol_pos_at_line++;
}


void LexicalScanner::float_num_state_changing(char symbol, std::string& lexeme)
{
	if (is_digit(symbol) || symbol == '.' || symbol == 'e' || symbol == '+' || symbol == '-')
	{
		lexeme += symbol;
		_symbol_pos_at_line++;
	}
	else if (symbol == '*' || symbol == '/')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER,
											_line_counter, _symbol_pos_at_line });
		lexeme = symbol;
		_current_state = States::ARITHM_OPERATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == '{')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER,
											_line_counter, _symbol_pos_at_line });
		lexeme = "";
		_current_state = States::COMMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == ';')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER,
											_line_counter, _symbol_pos_at_line });
		lexeme = symbol;
		_current_state = States::SEPARATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::FLOAT_NUMBER,
											_line_counter, _symbol_pos_at_line });
		lexeme = "";
		_current_state = States::START;
		if (symbol == '\n')
		{
			_line_counter++;
			_symbol_pos_at_line = 0;
		}
		else
			_symbol_pos_at_line++;
	}
	else
	{
		lexeme = "error on FLOAT_NUMBER state change.invalid character -> " + symbol;
		_current_state = States::ERROR;
		_symbol_pos_at_line++;
	}
}


void LexicalScanner::separator_state_changing(char symbol, std::string& lexeme)
{
	if (lexeme == "(")
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::LP_TOKEN, 
											_line_counter, _symbol_pos_at_line });
	else if (lexeme == ")")
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::RP_TOKEN,
											_line_counter, _symbol_pos_at_line });
	else if (lexeme == ";")
	{
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::SEMICOLON_TOKEN,
											_line_counter, _symbol_pos_at_line });
	}
	lexeme = "";

	if (is_letter(symbol))
	{
		lexeme += symbol;
		_current_state = States::ID;
		_symbol_pos_at_line++;
	}
	else if (is_digit(symbol))
	{
		lexeme += symbol;
		_current_state = States::FLOAT_NUMBER;
		_symbol_pos_at_line++;
	}
	else if (symbol == ';' || symbol == '(' || symbol == ')')
	{
		lexeme += symbol;
		_current_state = States::SEPARATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		lexeme += symbol;
		_current_state = States::ARITHM_OPERATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == '{')
	{
		_current_state = States::COMMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		lexeme = "";
		_current_state = States::START;
		if (symbol == '\n')
		{
			_code_line_counter++;
			_line_counter++;
			_symbol_pos_at_line = 0;
		}
		else
			_symbol_pos_at_line++;
	}
	else
	{
		lexeme = "error on SEPARATOR state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
		_symbol_pos_at_line++;
	}
}


void LexicalScanner::arithm_operator_state_changing(char symbol, std::string& lexeme)
{
	if (lexeme == "+")
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::PLUS_TOKEN,
											_line_counter, _symbol_pos_at_line });
	else if (lexeme == "-")
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::MINUS_TOKEN,
											_line_counter, _symbol_pos_at_line });
	else if (lexeme == "*")
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::STAR_TOKEN,
											_line_counter, _symbol_pos_at_line });
	else if (lexeme == "/")
		_lexeme_table->push_back(SyntaxToken{ lexeme, SyntaxTag::SLASH_TOKEN,
											_line_counter, _symbol_pos_at_line });

	lexeme = "";

	if (is_letter(symbol))
	{
		lexeme += symbol;
		_current_state = States::ID;
		_symbol_pos_at_line++;
	}
	else if (is_digit(symbol))
	{
		lexeme += symbol;
		_current_state = States::FLOAT_NUMBER;
		_symbol_pos_at_line++;
	}
	else if (symbol == '(' || symbol == ')')
	{
		lexeme += symbol;
		_current_state = States::SEPARATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == '{')
	{
		_current_state = States::COMMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == ' ' || symbol == '\t' || symbol == '\n')
	{
		lexeme = "";
		_current_state = States::START;
		if (symbol == '\n')
		{
			_line_counter++;
			_symbol_pos_at_line = 0;
		}
		else
			_symbol_pos_at_line++;
	}
	else
	{
		lexeme = "error on ARITHM_OPERATOR state change. invalid character -> " + symbol;
		_current_state = States::ERROR;
		_symbol_pos_at_line++;
	}
}


void LexicalScanner::comment_state_changing(char symbol, std::string& lexeme)
{
	if (symbol == '}')
	{
		lexeme = "";
		_current_state = States::START;
		_symbol_pos_at_line++;
	}
	else if (symbol == '\n')
	{
		lexeme = "not closed comment lexeme";
		_current_state = States::ERROR;
		_line_counter++;
		_symbol_pos_at_line = 0;
	}
}

void LexicalScanner::error_state_changing(char symbol, std::string& lexeme, std::vector<Error>& error_list)
{
	error_list.push_back(Error("current file", _error_level,
		ErrorTag::LEXICAL_ERROR,
		"lexer can't recognize character as an input language character. " + lexeme,
		_line_counter, _symbol_pos_at_line));
	lexeme = "";
	if (is_letter(symbol))
	{
		lexeme += symbol;
		_current_state = States::ID;
		_symbol_pos_at_line++;
	}
	else if (is_digit(symbol))
	{
		lexeme += symbol;
		_current_state = States::FLOAT_NUMBER;
		_symbol_pos_at_line++;
	}
	else if (symbol == ';' || symbol == '(' || symbol == ')')
	{
		lexeme += symbol;
		_current_state = States::SEPARATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
	{
		lexeme += symbol;
		_current_state = States::ARITHM_OPERATOR;
		_symbol_pos_at_line++;
	}
	else if (symbol == ':')
	{
		lexeme += symbol;
		_current_state = States::ASSIGNMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == '{')
	{
		_current_state = States::COMMENT;
		_symbol_pos_at_line++;
	}
	else if (symbol == '\n' || symbol == '\t' || symbol == ' ')
	{
		if (symbol == '\n')
		{
			_line_counter++;
			_symbol_pos_at_line = 0;
			_current_state = States::START;
		}
		else
			_symbol_pos_at_line++;
		return;
	}
	else
	{
		lexeme += symbol;
		_current_state = States::ERROR;
		_symbol_pos_at_line++;
	}
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

