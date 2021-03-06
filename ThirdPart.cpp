#include "ThirdPart.h"

void third_part()
{
	std::cout << " -> simple syntax analyzer" << std::endl;

	short selection = 4;
	std::string file;

	while (selection != 42)
	{
		file = "";

		std::cout << "\nyour turn to pick:\n"
			<< "\t1. select the file with lexem table.\n"
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
				Parser parser;
				parser.get_lexems_from_file(file);
				parser.parse();
			}
			catch (...) {}
			break;

		case 2:
			file = "demo_for_parser.txt";
			std::cout << "---------- demo ----------" << std::endl;
			try
			{
				Parser parser;
				parser.get_lexems_from_file(file);
				parser.parse();
			}
			catch (...) {}
			break;
		case 0:
			selection = 42;
		}

		std::cout << "<====================================================================================>" << std::endl;
	}

	std::cout << "\nend of the third part\n" << std::endl;
}

