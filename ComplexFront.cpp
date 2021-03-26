#include "ComplexFront.h"


ComplexFront::~ComplexFront()
{
	if (_parsed_ast)
	{
		for (auto&& el : *_parsed_ast)
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


bool ComplexFront::front_compile(std::string file)
{
	try
	{
		LexicalScanner lexer(file);
		_errors = new std::vector<Error>();
		_lexems = lexer.scan(*_errors);

		size_t num_of_lines = lexer.get_num_of_lines();

		Parser parser(_lexems, num_of_lines, file);
		_symbol_table = new HashMap<std::string, float>();
		
		_parsed_ast = parser.parse(*_symbol_table, *_errors);
		if (_errors->empty())
		{
			std::cout << "\n-> input file - " << file <<
				"\n-> front compiled successfuly" << std::endl;
			return true;
		}
		else
		{
			std::cout << "\n-> input file - " << file <<
				"\n-> front compile failed" << std::endl;
			print_error_list();
		}
	}
	catch (const std::runtime_error& runtime_error)
	{
		std::cout << "[error] - " << runtime_error.what() << std::endl;
	}
	catch (const std::exception& exception)
	{
		std::cout << "[error] - " << exception.what() << std::endl;
	}
	return false;
}


void ComplexFront::print_parsed_lines()
{
	if (_lexems)
	{
		size_t line{ 1 };
		std::cout << "\n-> source code lines\n";
		std::cout << line << ". ";
		for (size_t i = 0; i < _lexems->size(); i++)
		{
			std::cout << _lexems->at(i).lexeme;
			if (i < _lexems->size() - 1 && _lexems->at(i + 1).token_type != SyntaxTag::SEMICOLON_TOKEN)
				std::cout << " ";
			if (_lexems->at(i).token_type == SyntaxTag::SEMICOLON_TOKEN)
			{
				if (i < _lexems->size() - 1 && _lexems->at(i + 1).token_type != SyntaxTag::END_OF_FILE)
				{
					std::cout << std::endl;
					line++;
					std::cout << line << ". ";
				}
			}
		}
		std::cout << "\n" << std::endl;
	}
}


void ComplexFront::print_symbol_table()
{
	std::ios_base::fmtflags initial(std::cout.flags());
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
	std::cout.flags(initial);
}


void ComplexFront::print_error_list()
{
	std::ios_base::fmtflags initial(std::cout.flags());
	std::cout << "-> list of errors\n";
	std::cout << "---------"
		<< "----"
		<< "---------------"
		<< "----------------------------------------------------------------------------------------------------"
		<< "---------------"
		<< "----"
		<< "------"
		<< "---\n";
	std::cout << "code";
	std::cout << "  |";
	std::cout << "\t";
	std::cout.width(15);
	std::cout.fill(' ');
	std::cout << std::left << "error type";
	std::cout << "| ";
	std::cout.width(100);
	std::cout.fill(' ');
	std::cout << std::left << "message";
	std::cout << "| ";
	std::cout.width(15);
	std::cout.fill(' ');
	std::cout << "file name";
	std::cout << "| ";
	std::cout.width(4);
	std::cout.fill(' ');
	std::cout << "line";
	std::cout.width(3);
	std::cout.fill(' ');
	std::cout << "| ";
	std::cout << "pos";
	std::cout << "|\n";
	std::cout << "---------"
		<< "----"
		<< "---------------"
		<< "----------------------------------------------------------------------------------------------------"
		<< "---------------"
		<< "----"
		<< "------"
		<< "---\n";
	for (auto&& er : *_errors)
	{
		er.print();
	}
	std::cout << "---------"
		<< "----"
		<< "---------------"
		<< "----------------------------------------------------------------------------------------------------"
		<< "---------------"
		<< "----"
		<< "------"
		<< "---\n";

	std::cout.flags(initial);
}


void ComplexFront::draw_all_trees()
{
	if (_parsed_ast)
	{
		AstDrawer drawer;
		std::cout << "\n-> parsed trees\n" << std::endl;
		for (auto&& ast : *_parsed_ast)
		{
			drawer.draw_tree(ast, true);
			std::cout << "\n" << std::endl;
		}
	}
}

