#include "ComplexFront.h"


ComplexFront::~ComplexFront()
{
	if (_parsed_ast)
	{
		for (auto& el : *_parsed_ast)
		{
			if (el)
				delete el;
		}
		delete _parsed_ast;
	}
	if (_lexems)
		delete _lexems;
	if (_symbol_table)
		delete _symbol_table;
}


void ComplexFront::front_compile(std::string file)
{
	try
	{
		LexicalScanner lexer(file);
		_lexems = lexer.scan();

		size_t num_of_lines = lexer.get_num_of_lines();

		Parser parser(_lexems, num_of_lines);
		_symbol_table = new HashMap<std::string, float>();
		_parsed_ast = parser.parse(*_symbol_table);

		std::cout << "front compiled successfuly" << std::endl;
	}
	catch (const std::runtime_error & runtime_error)
	{
		std::cout << "[error] - " << runtime_error.what() << std::endl;
	}
	catch (const std::exception & exception)
	{
		std::cout << "[error] - " << exception.what() << std::endl;
	}
}

void ComplexFront::print_parsed_lines()
{
	if (_lexems)
	{
		std::cout << "source code lines:\n";
		for (size_t i = 0; i < _lexems->size(); i++)
		{
			std::cout << _lexems->at(i).lexeme << " ";
			if (_lexems->at(i).token_type == SyntaxTag::SEMICOLON_TOKEN)
				std::cout << std::endl;
		}
	}
}


void ComplexFront::print_symbol_table()
{
	if (_symbol_table)
	{
		std::cout << " -> symbol table <- " << std::endl;
		std::cout << " ----------------------------------------------------------------------- " << std::endl;
		std::cout << "|           key         |         value         |        datatype       |" << std::endl;
		std::cout << " ----------------------------------------------------------------------- " << std::endl;
		auto symbol_arr = _symbol_table->get_all_id();
		for (size_t i = 0; i < _symbol_table->get_size(); i++)
		{
			if (symbol_arr[i])
			{
				std::cout << "| ";
				std::cout.width(17);
				std::cout.fill(' ');
				std::cout << symbol_arr[i]->_key;
				std::cout.width(7);
				std::cout.fill();
				std::cout << " | ";
				std::cout.width(17);
				std::cout.fill(' ');
				std::cout << symbol_arr[i]->_value;
				std::cout.width(7);
				std::cout.fill(' ');
				std::cout << " | ";
				std::cout.width(17);
				std::cout.fill(' ');
				std::cout << symbol_arr[i]->_datatype;
				std::cout.width(7);
				std::cout.fill(' ');
				std::cout << " | " << std::endl;
			}
		}
		std::cout << " ----------------------------------------------------------------------- " << std::endl;
	}
}


void ComplexFront::print_line_and_tree()
{
	if (_lexems && _parsed_ast)
	{
		size_t line{ 0 };
		AstDrawer drawer;
		for (size_t i = 0; i < _lexems->size(); i++)
		{
			std::cout << _lexems->at(i).lexeme << " ";
			if (_lexems->at(i).token_type == SyntaxTag::SEMICOLON_TOKEN)
			{
				std::cout << std::endl;
				std::cout << "\nast of the line: " << std::endl;
				drawer.draw_tree(_parsed_ast->at(line), true);
				line++;
				std::cout << std::endl;
			}
		}
	}
}


void ComplexFront::draw_all_trees()
{
	if (_parsed_ast)
	{
		AstDrawer drawer;
		std::cout << "parsed ast trees:\n";
		for (auto& sentence : *_parsed_ast)
		{
			drawer.draw_tree(sentence, true);
			std::cout << std::endl;
		}
	}
}

