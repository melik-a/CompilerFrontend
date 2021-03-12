#include <iostream>
#include "ComplexFront.h"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
/**
*** to do:	add error handling, add evaluating, add id declaration,
***			rework some classes to provide little bit oop design,
***			rework main() - imagine some useful functionality.
***
**/

int main()
{
	system("chcp 1251");
	std::wcout << " -> front part of simple compiler." << std::endl;
	short selection = 4;
	std::string file;

	while (selection != 42)
	{
		file = "";

		std::cout << "\nyour turn to pick:\n"
			<< "\t1. front compile from the source code file.\n"
			<< "\t2. console input front compile.\n"
			<< "\t3. demonstration.\n"
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
			std::cout << file << std::endl;
			try
			{
				ComplexFront front;
				front.front_compile(file);
				front.print_parsed_lines();
				front.print_symbol_table();
				front.print_line_and_tree();
				front.draw_all_trees();
			}
			catch (...) {}
			break;
		case 2:
			std::cout << "\ninput source code -> ";
			try
			{
				/*
				ComplexFront front;
				front.front_compile(file);
				front.print_symbol_table();
				front.print_line_and_tree();*/
			}
			catch (...) {}
			break;
		case 3:
			try
			{
				ComplexFront front;
				front.front_compile("demo.txt");
				front.print_parsed_lines();
				front.print_symbol_table();
				front.print_line_and_tree();
				front.draw_all_trees();
			}
			catch (...) {}
			break;
		case 0:
			selection = 42;
		}

		std::cout << "<====================================================================================>" << std::endl;
	}

	std::cout << "\nbye, bye\n" << std::endl;

	system("pause");
	return 0;
}
