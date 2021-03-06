#include "FirstPart.h"


Pair<double, double> manual_input(HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list)
{
	Pair<double, double> cmp(0, 0);
	int count_of_entries{ 0 };
	std::string key, value;
	std::cout << "manual input. type \"exit exit\" to stop" << std::endl;
	std::cout << " enter key and value separated by space " << std::endl;
	std::cout << " ----------------------------------- " << std::endl;
	std::cout << "|        key      |      value      |" << std::endl;
	std::cout << " ----------------------------------- " << std::endl;
	while (true)
	{
		key = "";
		value = "";
		std::cin >> key >> value;
		if (key == "exit" and value == "exit")
			break;
		table.insert(key, value);
		list.push_front(key, value);
		cmp._first += table.get_last_number_of_comparisons();
		cmp._second += list.get_last_number_of_comparisons();
		count_of_entries++;
	}
	std::cout << " -> your symbol table <- " << std::endl;
	std::cout << " ----------------------------------- " << std::endl;
	std::cout << "|        key      |      value      |" << std::endl;
	std::cout << " ----------------------------------- " << std::endl;
	auto node = list.front();
	while (node)
	{
		std::cout << "| ";
		std::cout.width(10);
		std::cout.fill(' ');
		std::cout << node->_name;
		std::cout.width(8);
		std::cout.fill();
		std::cout << " | ";
		std::cout.width(10);
		std::cout.fill(' ');
		std::cout << node->_info;
		std::cout.width(8);
		std::cout.fill(' ');
		std::cout << " | " << std::endl;
		node = node->_next;
	}

	cmp._first = cmp._first / count_of_entries;
	cmp._second = cmp._second / count_of_entries;
	return cmp;
}


Pair<double, double> tables_from_file(const std::string& path, HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list)
{
	Pair<double, double> cmp(0, 0);
	int count_of_entries{ 0 };
	try
	{
		std::ifstream file_input(path);
		if (!file_input)
			throw "error";
		std::string key, value;
		while (!file_input.eof())
		{
			key = "";
			value = "";
			file_input >> key >> value;
			table.insert(key, value);
			list.push_front(key, value);
			cmp._first += table.get_last_number_of_comparisons();
			cmp._second += list.get_last_number_of_comparisons();
			count_of_entries++;
		}

		file_input.close();

		cmp._first = cmp._first / count_of_entries;
		cmp._second = cmp._second / count_of_entries;
	}
	catch (...)
	{
		std::cout << "\nsomething went wrong. invalid file name or path" << std::endl;
		throw;
	}

	return cmp;
}


Pair<std::string, std::string> search_in_tables(const std::string& key, HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list)
{
	Pair<std::string, std::string> result;

	size_t index = table.search_pair(key);
	if (index != table.get_size())
		result._first = table[index]->_first;
	else
		result._first = "nothing found";

	auto node = list.search(key);
	if (node)
		result._second = node->_name;
	else
		result._second = "nothing found";

	return result;
}


void user_look_up(HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list)
{
	Pair<std::string, std::string> search_results;
	std::string answer_and_key;
	while (true)
	{
		std::cout << "\nlooking for something? {y/n} -> ";
		std::cin >> answer_and_key;
		std::transform(answer_and_key.begin(), answer_and_key.end(), answer_and_key.begin(), tolower);
		if (answer_and_key == "y" || answer_and_key == "yes")
		{
			std::cout << "\nand what exactly? -> ";
			std::cin >> answer_and_key;
			search_results = search_in_tables(answer_and_key, table, list);

			std::cout << "\nhash table -> search result {key} => " << search_results._first << std::endl;
			std::cout << "hash table -> number of comparisons in the search => "
				<< table.get_last_number_of_comparisons() << std::endl;

			std::cout << "list table -> search result {key} => " << search_results._second << std::endl;
			std::cout << "list table -> number of comparisons in the search => "
				<< list.get_last_number_of_comparisons() << std::endl;
		}
		else
			break;
	}
}


void first_part()
{
	std::cout << "..." << std::endl;
	std::cout << "this program implements two structures: linked list and hash table" << std::endl;
	std::cout << "the program compares these methods of constructing symbol tables. " << std::endl;

	Pair<double, double> mean_comparisons;
	short selection = 4;
	std::string file;

	while (selection != 42)
	{
		LinkedList<std::string, std::string> list_table;
		HashMap<std::string, std::string> hash_table;
		file = "";

		std::cout << "\nyour turn to pick:\n"
			<< "\t1. manually input the content of symbol table.\n"
			<< "\t2. select the file with content of symbol table.\n"
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
			mean_comparisons = manual_input(hash_table, list_table);

			std::cout << "\nthere is results -> mean number of ñomparisons on inserting elements:\n"
				<< "hash table - " << mean_comparisons._first << ", list table - " << mean_comparisons._second << std::endl;
			user_look_up(hash_table, list_table);
			break;

		case 2:
			std::cout << "\ninput the path to file {C:\\TEMP\\example.txt} -> ";
			std::cin >> file;
			try
			{
				mean_comparisons = tables_from_file(file, hash_table, list_table);
				std::cout << "\nhash table => ";
				hash_table.print_all_entries();
				std::cout << "\nlist table => ";
				list_table.print();

				std::cout << "\nthere is results -> mean number of ñomparisons on inserting elements:\n"
					<< "hash table - " << mean_comparisons._first << ", list table - " << mean_comparisons._second << std::endl;
				user_look_up(hash_table, list_table);
			}
			catch (...) {}
			break;

		case 3:
			file = "demo_table.txt";
			mean_comparisons = tables_from_file(file, hash_table, list_table);
			std::cout << "\nhash table => ";
			hash_table.print_all_entries();
			std::cout << "\nlist table => ";
			list_table.print();

			std::cout << "\nthere is results -> mean number of ñomparisons on inserting elements:\n"
				<< "hash table - " << mean_comparisons._first << ", list table - " << mean_comparisons._second << std::endl;
			user_look_up(hash_table, list_table);
			break;
		case 0:
			selection = 42;
		}

		std::cout << "<====================================================================================>" << std::endl;
	}

	std::cout << "\nend of the first part\n" << std::endl;
}