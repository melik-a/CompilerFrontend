#include "ComplexFront.h"


std::ostream& operator << (std::ostream& os, SyntaxToken tok)
{
	os << " <" << tok.lexeme << ", " << tok.token_type_to_string() << "> ";
	return os;
}


void ComplexFront::code_to_ast(std::string file)
{
	LexicalScanner lexer(file);
	auto scan = lexer.scan();
	size_t num_of_lines = lexer.get_num_of_lines();
	
	Parser parser(scan, num_of_lines);
	_parsed_ast = parser.parse();
	
	std::cout << "source code lines:\n";
	for (int i = 0; i < scan->size(); i++)
	{
		std::cout << scan->at(i).lexeme << " ";
		if (scan->at(i).token_type == SyntaxTag::SEMICOLON_TOKEN)
			std::cout << std::endl;
	}
}


void ComplexFront::draw_ast()
{
	AstDrawer drawer;
	std::cout << "parsed ast trees:\n";
	for (auto sentence : *_parsed_ast)
	{
		drawer.draw_tree(sentence, true);
		std::cout << std::endl;
	}
}

