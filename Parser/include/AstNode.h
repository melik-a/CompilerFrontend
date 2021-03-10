#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "AstTag.h"
#include "SyntaxTag.h"


struct AstNode
{
	AstNode();
	AstNode(AstTag tag);
	AstNode(AstTag tag, AstNode& trans, AstNode& add_sub);
	~AstNode();

	void add_child(AstNode* child);
	AstNode* get_child(int index);
	int num_of_children();
	std::string node_type_to_string();
	float get_value();

	virtual void print_node()
	{
		std::cout << " <" << node_type_to_string() << ">";
	}

	AstTag _node_type;
	float _value; // optional, possible can be use for evaluation

	private:
		std::vector<AstNode*> _children;
};

