#pragma once
#include <iostream>
#include "Node.h"


template <typename N, typename I>
struct LinkedList
{
	explicit LinkedList() :
		_head(nullptr), _size(0) {}


	~LinkedList()
	{
		while (_head)
			pop_front();
	}

	void clear() noexcept;
	bool pop_front() noexcept;

	void push_front(const Node<N, I>& new_node);
	void push_front(Node<N, I>&& new_node);
	void push_front(const N& name, const I& info);
	void push_front(N&& name, I&& info);

	const Node<N, I>* search(const Node<N, I>& s_node);
	const Node<N, I>* search(Node<N, I>&& s_node);
	const Node<N, I>* search(const N& s_name);
	const Node<N, I>* search(N&& s_name);

	const Node<N, I>* front() const;

	void print() const;

	bool empty() const;
	size_t get_size() const;
	size_t get_last_number_of_comparisons() const;

	const Node<N, I>* operator[](const N& elem);
	const Node<N, I>* operator[](N&& elem);

	private:	
		Node<N, I>* _head;
		size_t _size;
		size_t _last_num_comparisons{};
};


template <typename N, typename I>
std::ostream& operator<<(std::ostream& os, const LinkedList<N, I>& list)
{
	auto node = list.front();
	while (node)
	{
		os << "{" << node->_name << " - " << node->_info << "}";
		node = node->_next;
		if (node)
			std::cout << " -> ";
	}
	return os;
}


template <typename N, typename I>
const Node<N, I>* LinkedList<N, I>::operator[](const N& elem)
{ 
	return this->search(elem);
}


template <typename N, typename I>
const Node<N, I>* LinkedList<N, I>::operator[](N&& elem)
{
	return this->search(std::move(elem));
}


template <typename N, typename I>
const Node<N, I>* LinkedList<N, I>::front() const
{
	return _head;
}


template <typename N, typename I>
void LinkedList<N, I>::print() const
{
	Node<N, I>* node = _head;
	while (node)
	{
		std::cout << "{" << node->_name << " - " << node->_info << "}";
		node = node->_next;
		if (node)
			std::cout << " -> ";
	}
	std::cout << std::endl;
}


template <typename N, typename I>
void LinkedList<N, I>::clear() noexcept
{
	while (_head)
		pop_front();
	_size = 0;
}


template <typename N, typename I>
const Node<N, I>* LinkedList<N, I>::search(const Node<N, I>& s_node)
{
	_last_num_comparisons = 0;
	Node<N, I>* node = _head;
	while (node)
	{
		_last_num_comparisons++;
		if (node->_name == s_node._name)
			return node;
		node = node->_next;
	}
	return node;
}


template <typename N, typename I>
const Node<N, I>* LinkedList<N, I>::search(Node<N, I>&& s_node)
{
	_last_num_comparisons = 0;
	Node<N, I>* node = _head;
	while (node)
	{
		_last_num_comparisons++;
		if (node->_name == s_node._name)
			return node;
		node = node->_next;
	}
	return node;
}


template <typename N, typename I>
const Node<N, I>* LinkedList<N, I>::search(const N& s_name)
{
	_last_num_comparisons = 0;
	Node<N, I>* node = _head;
	while (node)
	{
		_last_num_comparisons++;
		if (node->_name == s_name)
			return node;
		node = node->_next;
	}
	return node;
}


template <typename N, typename I>
const Node<N, I>* LinkedList<N, I>::search(N&& s_name)
{
	_last_num_comparisons = 0;
	Node<N, I>* node = _head;
	while (node)
	{
		_last_num_comparisons++;
		if (node->_name == s_name)
			return node;
		node = node->_next;
	}
	return node;
}


template <typename N, typename I>
void LinkedList<N, I>::push_front(const Node<N, I>& new_node)
{
	Node<N, I>* node = new Node<N, I>(new_node);
	node->_next = this->_head;
	this->_head = node;
	_size += 1;
}


template <typename N, typename I>
void LinkedList<N, I>::push_front(Node<N, I>&& new_node)
{
	Node<N, I>* node = new Node<N, I>(std::move(new_node));
	node->_next = this->_head;
	this->_head = node;
	_size += 1;
}


template <typename N, typename I>
void LinkedList<N, I>::push_front(const N& name, const I& info)
{
	Node<N, I>* node = new Node<N, I>(name, info);
	node->_next = this->_head;
	this->_head = node;
	_size += 1;
}


template <typename N, typename I>
void LinkedList<N, I>::push_front(N&& name, I&& info)
{
	Node<N, I>* node = new Node<N, I>(std::move(name), std::move(info));
	node->_next = this->_head;
	this->_head = node;
	_size += 1;
}


template <typename N, typename I>
bool LinkedList<N, I>::pop_front() noexcept
{
	if (_head)
	{
		Node<N, I>* new_head = this->_head->_next;
		delete _head;
		_head = new_head;
		_size -= 1;
		return true;
	}
	return false;
}


template <typename N, typename I>
size_t LinkedList<N, I>::get_size() const
{
	return _head ? 0 : _size;
}


template <typename N, typename I>
bool LinkedList<N, I>::empty() const
{
	return _head ? false : true;
}


template <typename N, typename I>
size_t LinkedList<N, I>::get_last_number_of_comparisons() const
{
	return _last_num_comparisons;
}
