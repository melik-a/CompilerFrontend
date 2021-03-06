#pragma once

#include <string>
#include <algorithm>
#include <fstream>

#include "SymbolTable/HashMap/HashMap.h"
#include "SymbolTable/List/ListTable.h"

Pair<double, double> manual_input(HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list);
Pair<double, double> tables_from_file(const std::string& path, HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list);
Pair<std::string, std::string> search_in_tables(const std::string& key, HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list);
void user_look_up(HashMap<std::string, std::string>& table, LinkedList<std::string, std::string>& list);
void first_part();

