#include "SecondPart.h"

void print_lexeme_table(LexicalScanner& table)
{
	std::cout << "				 -> lexeme table <- " << std::endl;
	std::cout << " ------------------------------------------------- " << std::endl;
	std::cout << "|          lexeme        |        lexeme_type     |" << std::endl;
	std::cout << " ------------------------------------------------- " << std::endl;

	auto lexemes = table.scan();

	for (auto el : *lexemes)
	{
		std::cout << "| ";
		std::cout.width(15);
		std::cout.fill(' ');
		std::cout << el.lexeme;
		std::cout.width(10);
		std::cout.fill();
		std::cout << " | ";
		std::cout.width(15);
		std::cout.fill(' ');
		std::cout.right;
		std::cout << el.token_type_to_string();
		std::cout.width(10);
		std::cout.fill(' ');
		std::cout << " | " << std::endl;
	}
	std::cout << " ------------------------------------------------- " << std::endl;
}

void second_part()
{
	std::cout << " -> simple lexical analyzer" << std::endl;

	short selection = 4;
	std::string file;

	while (selection != 42)
	{
		file = "";

		std::cout << "\nyour turn to pick:\n"
			<< "\t1. select the file with source code.\n"
			<< "\t2. demonstration.\n"
			<< "\t0. exit" << std::endl;
		std::cout << "\nyour pick -> ";
		std::cin >> selection;
		if (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			continue;
		}

		switch (selection)
		{
		case 1:
			std::cout << "\ninput the path to file {C:\\TEMP\\example.txt} -> ";
			std::cin >> file;
			try
			{
				LexicalScanner lex_scanner(file);
				print_lexeme_table(lex_scanner);
			}
			catch (...) {}
			break;

		case 2:
			file = "demo_for_lexer.txt";
			std::cout << "---------- demo ----------" << std::endl;
			try
			{
				LexicalScanner lex_scanner(file);
				print_lexeme_table(lex_scanner);
			}
			catch (...) {}
			break;
		case 0:
			selection = 42;
		}

		std::cout << "<====================================================================================>" << std::endl;
	}

	std::cout << "\nend of second part\n" << std::endl;
}

