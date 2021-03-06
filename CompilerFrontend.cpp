
#include "FirstPart.h"
#include "SecondPart.h"
#include "ThirdPart.h"

int main()
{
	setlocale(LC_ALL, "rus");
	
	std::cout << " -> front part of simple compiler." << std::endl;
	std::cout << "in separate parts for a while, but it's not for long time, i hope <-" << std::endl;
	short selection = 4;
	std::string file;

	while (selection != 42)
	{
		file = "";

		std::cout << "\nyour turn to pick:\n"
			<< "\t1. first part - symbol table.\n"
			<< "\t2. second part - lexical analyzer.\n"
			<< "\t3. third part - syntax analyzer.\n"
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
			first_part();
			break;
		case 2:
			second_part();
			break;
		case 3:
			third_part();
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
