#pragma once

template <typename N, typename I>
struct Node
{
	N _name;
	I _info;
	Node* _next;

	Node() :
		_next(nullptr) {}

	Node(const N& name, const I& info) :
		_next(nullptr), _name(name), _info(info) {}

	Node(N&& name, I&& info) noexcept :
		_next(nullptr), _name(std::move(name)), _info(std::move(info)) {}

	Node(const Node<N, I>& node) :
		_next(node._next), _name(node._name), _info(node._info) {}
	
	Node(Node<N, I>&& node) noexcept :
		_next(std::move(node._next)), _name(std::move(node._name)), _info(std::move(node._info)) {}

	~Node(){}
};

