#include "ComplexFront.h"


void ComplexFront::front_compile(std::string file)
{
	LexicalScanner lexer(file);
	_lexems = lexer.scan();
	size_t num_of_lines = lexer.get_num_of_lines();
	
	Parser parser(_lexems, num_of_lines);
	_parsed_ast = parser.parse(_symbol_table);
	std::cout << "front compiled successfuly" << std::endl;
}

void ComplexFront::print_parsed_lines()
{
	std::cout << "source code lines:\n";
	for (int i = 0; i < _lexems->size(); i++)
	{
		std::cout << _lexems->at(i).lexeme << " ";
		if (_lexems->at(i).token_type == SyntaxTag::SEMICOLON_TOKEN)
			std::cout << std::endl;
	}
}

void ComplexFront::print_symbol_table()
{
	std::cout << "symbol table: " << std::endl;
	_symbol_table.print_all_entries();
	std::cout << std::endl;
}

void ComplexFront::print_line_and_tree()
{
	size_t line{ 0 };
	AstDrawer drawer;
	for (int i = 0; i < _lexems->size(); i++)
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


void ComplexFront::draw_all_trees()
{
	AstDrawer drawer;
	std::cout << "parsed ast trees:\n";
	for (auto sentence : *_parsed_ast)
	{
		drawer.draw_tree(sentence, true);
		std::cout << std::endl;
	}
}

