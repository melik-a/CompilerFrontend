#include "../include/AstNode.h"


AstNode::AstNode() : _node_type(AstTag::SYNTAX_TOKEN) {}

AstNode::AstNode(AstTag tag) : _node_type(tag) {}

AstNode::AstNode(AstTag tag, AstNode& trans, AstNode& add_sub) : _node_type(tag)
{
	_children.push_back(&trans);
	_children.push_back(&add_sub);
}

AstNode::~AstNode()
{
	for (auto& el : _children)
	{
		if (el)
			delete el;
	}
}


float AstNode::get_value()
{
	return _value;
}


void AstNode::add_child(AstNode* child)
{
	_children.push_back(child);
}


AstNode* AstNode::get_child(int index)
{
	return _children[index];
}


int AstNode::num_of_children()
{
	return _children.size();
}


std::string AstNode::node_type_to_string()
{
	switch (_node_type)
	{
		case AstTag::STMT:
			return "STMT";
			break;
		case AstTag::SYNTAX_TOKEN:
			return "SYNTAX_TOKEN";
			break;
		case AstTag::EXPRESSION:
			return "EXPRESSION";
			break;
		case AstTag::ADD_SUB:
			return "ADD_SUB";
			break;
		case AstTag::FACTOR:
			return "FACTOR";
			break;
		case AstTag::TRANS:
			return "TRANS";
			break;
		case AstTag::MUL_DIV:
			return "MUL_DIV";
			break;
		case AstTag::ARR_SIZE:
			return "ARR_SIZE";
			break;
		case AstTag::BLOCK:
			return "BLOCK";
			break;
		case AstTag::BASIC_TYPE:
			return "BASIC_TYPE";
			break;
		case AstTag::DECL:
			return "DECL";
			break;
		case AstTag::DECL_LIST:
			return "DECL_LIST";
			break;
		case AstTag::GLOBAL:
			return "GLOBAL";
			break;
		case AstTag::ID_LIST:
			return "ID_LIST";
			break;
		case AstTag::VAR_DECL:
			return "VAR_DECL";
			break;
		case AstTag::TYPE:
			return "TYPE";
			break;
		case AstTag::PROG_DEF:
			return "PROG_DEF";
			break;
		case AstTag::PROGRAM:
			return "PROGRAM";
			break;
	}
	return "UNKNOWN_AST_TAG";
}
