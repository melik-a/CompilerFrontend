#pragma once

#include <iostream>
#include "Hash.h"
#include "Pair.h"
#include <cassert>

/***  
 *** simple implementation of hash table
 *** template arguments: K - type of storage key,
 ***					 V - type of storage value,
 ***					 H - hash function (by dedault used Hash<K>{})
 *** 
 *** implementation provides some general methods: insert, remove, search, upgrade, resize and etc.
 *** 
 *** to use this hash table implementaion with some non-trivial structures
 *** need to transfer as a template argument some hash functions that can provide hashing this structure.
 *** 
 *** also hash table supports move semantics.
 *** 
 ***/

template <typename K, typename V, typename H = Hash<K>>
struct HashMap
{
	HashMap() : 
		_size(_default_size), _mass(new Pair<K, V>* [_size] { nullptr }) {}
	
	HashMap(size_t size) :
		_size(size), _mass(new Pair<K, V>* [_size] { nullptr }) {}

	~HashMap()
	{
		for (size_t i = 0; i < _size; i++)
			delete _mass[i];
		delete _mass;
	}
	
	bool is_empty() const;
	bool is_full() const;
	double load_factor() const;
	double load_factor_threshold() const;
	size_t default_size() const;

	void print_all_entries() const;
	void print_all_keys() const;
	void print_all_values() const;

	Pair<K, V>* const* get_pairs() const;

	bool insert(const K& key, const V& value);
	bool insert(K&& key, V&& value);
	bool insert(Pair<K, V>&& new_pair);
	bool insert(const Pair<K, V>& new_pair);

	size_t search_pair(const K& key);
	size_t search_pair(K&& key);

	bool remove_pair(const K& key);
	bool remove_pair(K&& key);

	bool upgrade(const K& key, V&& value);
	bool upgrade(K&& key, V&& value);

	size_t get_number_of_entries() const;
	size_t get_size() const;
	size_t get_last_number_of_comparisons() const;

	const Pair<K, V>* operator[](const size_t& index) const;
	const Pair<K, V>* operator[](const K& key);
	const Pair<K, V>* operator[](K&& key);

	private:
		static constexpr size_t _default_size = 200;
		static constexpr float _load_factor_threshold = 0.6f;
		static constexpr short _growth_factor = 2;

		size_t _size;
		Pair<K, V> **_mass;
		H _hash{};
		size_t _number_of_entries{};
		size_t _last_num_comparisons{};

		void resize_table();
};


template <typename K, typename V, typename H>
std::ostream& operator << (std::ostream& os, const HashMap<K, V, H>& table)
{
	os << "\n{ ";
	size_t size = table.get_size();
	for (size_t i = 0; i < size; i++)
	{
		if (table[i])
			os << table[i]->_first << " : " << table[i]->_second;
		else
			os << " : ";
		
		if (i < size - 1)
			std::cout << ", ";
	}
	os << " }\n";
	return os;
}


template <typename K, typename V, typename H>
const Pair<K, V>* HashMap<K, V, H>::operator[](const size_t& index) const
{
	assert(index < _size);
	return _mass[index];
}


template <typename K, typename V, typename H>
const Pair<K, V>* HashMap<K, V, H>::operator[](const K& key)
{
	size_t index = search_pair(key);
	return index == get_size() ? nullptr : _mass[index];
}


template <typename K, typename V, typename H>
const Pair<K, V>* HashMap<K, V, H>::operator[](K&& key)
{
	size_t index = this->search_pair(std::move(key));
	return index == get_size() ? nullptr : _mass[index];
}


template <typename K, typename V, typename H>
size_t HashMap<K, V, H>::get_number_of_entries() const
{
	return _number_of_entries;
}


template <typename K, typename V, typename H>
size_t HashMap<K, V, H>::get_size() const
{
	return _size;
}


template <typename K, typename V, typename H>
size_t HashMap<K, V, H>::get_last_number_of_comparisons() const
{
	return _last_num_comparisons;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::is_empty() const
{
	return _number_of_entries ? false : true;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::is_full() const
{
	return _number_of_entries == _size ? true : false;
}


template <typename K, typename V, typename H>
double HashMap<K, V, H>::load_factor() const
{
	return (double)_number_of_entries / _size;
}


template <typename K, typename V, typename H>
double HashMap<K, V, H>::load_factor_threshold() const
{
	return _load_factor_threshold;
}


template <typename K, typename V, typename H>
size_t HashMap<K, V, H>::default_size() const
{
	return _default_size;
}


template <typename K, typename V, typename H>
void HashMap<K, V, H>::print_all_entries() const
{
	if (is_empty())
	{
		std::cout << "\n[ all pairs are empty. there is nothing to print ]" << std::endl;
		return;
	}
	std::cout << "{ ";
	for (size_t i = 0; i < _size; i++)
	{
		if (_mass[i])
		{
			std::cout << "{ " << _mass[i]->_first << " : " << _mass[i]->_second << " }";
			if (i < _size - 1)
				std::cout << " ";
		}
	}
	std::cout << "}" << std::endl;
}


template <typename K, typename V, typename H>
void HashMap<K, V, H>::print_all_keys() const
{
	if (is_empty()) 
	{
		std::cout << "\n[ all pairs are empty. there is nothing to print ]" << std::endl;
		return;
	}
	std::cout << "\n{";
	for (size_t i = 0; i < _size; i++)
	{
		if (_mass[i])
			std::cout << _mass[i]->_first;
		else
			std::cout << "nullptr";

		if (i < _size - 1)
			std::cout << ", ";
	}
	std::cout << "}" << std::endl;
}


template <typename K, typename V, typename H>
void HashMap<K, V, H>::print_all_values() const
{
	if (is_empty())
	{
		std::cout << "\n[ all pairs are empty. there is nothing to print ]" << std::endl;
		return;
	}
	std::cout << "\n{"; 
	for (size_t i = 0; i < _size; i++)
	{
		if (_mass[i])
			std::cout << _mass[i]->_second;
		else
			std::cout << "nullptr";
		
		if (i < _size - 1)
			std::cout << ", ";
	}
	std::cout << "}" << std::endl;
}


template <typename K, typename V, typename H>
Pair<K, V>* const* HashMap<K, V, H>::get_pairs() const
{
	return _mass;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::insert(const K& key, const V& value)
{
	if (_load_factor_threshold < load_factor())
		resize_table();
	
	_last_num_comparisons = 0;

	size_t index = _hash(key) % _size;
	for (size_t i = 1; (i < _size) && (_mass[index]); i++)
	{
		index = (_hash(key) + i) % _size;
		_last_num_comparisons++;
	}

	_last_num_comparisons++;
	if (!_mass[index])
	{
		_mass[index] = new Pair<K, V> (key, value);
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::insert(K&& key, V&& value)
{
	if (_load_factor_threshold < load_factor())
		resize_table();

	_last_num_comparisons = 0;

	size_t index = _hash(std::move(key)) % _size;
	for (size_t i = 1; (i < _size) && (_mass[index]); i++)
	{
		index = (_hash(std::move(key)) + i) % _size;
		_last_num_comparisons++;
	}

	_last_num_comparisons++;
	if (!_mass[index])
	{
		_mass[index] = new Pair<K, V>(std::move(key), std::move(value));
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::insert(Pair<K, V>&& new_pair)
{
	if (_load_factor_threshold < load_factor())
		resize_table();

	_last_num_comparisons = 0;

	size_t index = _hash(std::move(new_pair._first)) % _size;
	for (size_t i = 1; (i < _size) && (_mass[index]); i++)
	{
		index = (_hash(std::move(new_pair._first)) + i) % _size;
		_last_num_comparisons++;
	}

	_last_num_comparisons++;
	if (!_mass[index])
	{
		_mass[index] = new Pair<K, V>(std::move(new_pair));		
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::insert(const Pair<K, V>& new_pair)
{
	if (_load_factor_threshold < load_factor())
		resize_table();

	_last_num_comparisons = 0;

	size_t index = _hash(new_pair._first) % _size;
	for (size_t i = 1; (i < _size) && (_mass[index]); i++)
	{
		index = (_hash(new_pair._first) + i) % _size;
		_last_num_comparisons++;
	}

	_last_num_comparisons++;
	if (!_mass[index])
	{
		_mass[index] = new Pair<K, V>(new_pair);
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename H>
size_t HashMap<K, V, H>::search_pair(const K& key)
{
	_last_num_comparisons = 0;

	size_t index = _hash(key) % _size;

	// if elem is empty -> didn't find elem, else check keys on equal
	bool find = (!_mass[index] ? false : _mass[index]->_first == key);
	for (size_t i = 1; (i < _size) && (!find); i++)
	{
		index = (_hash(key) + i) % _size;
		_last_num_comparisons++;
		find = (!_mass[index] ? false : _mass[index]->_first == key);
	}

	_last_num_comparisons++;
	if (find)
		return index;
	return _size;
}


template <typename K, typename V, typename H>
size_t HashMap<K, V, H>::search_pair(K&& key)
{
	_last_num_comparisons = 0;

	size_t index = _hash(std::move(key)) % _size;

	// if elem is empty -> didn't find elem, else check keys on equal
	bool find = (!_mass[index] ? false : _mass[index]->_first == key);
	for (size_t i = 1; (i < _size) && (!find); i++)
	{
		index = (_hash(std::move(key)) + i) % _size;
		_last_num_comparisons++;
		find = (!_mass[index] ? false : _mass[index]->_first == key);
	}
	// equivalent search
	/*size_t i{0};
	while (!find) {
		index = simple_rehash(key, i);
		_last_num_comparisons++;
		find = (_mass[index] ? false : _mass[index]->_first == key);
		i++;
	}*/

	_last_num_comparisons++;
	if (find)
		return index;
	return _size;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::remove_pair(const K& key)
{
	size_t index = search_pair(key);
	if (index != _size)
	{
		_number_of_entries--;
		_mass[index] = nullptr;
		return true;
	}
	return false;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::remove_pair(K&& key)
{
	size_t index = search_pair(std::move(key));
	if (index != _size)
	{
		_number_of_entries--;
		_mass[index] = nullptr;
		return true;
	}
	return false;
}


template <typename K, typename V, typename H>
void HashMap<K, V, H>::resize_table()
{
	size_t new_size = _size * _growth_factor;
	Pair<K, V> **new_mass = new Pair<K, V>*[new_size] { nullptr };
	size_t index{};
	Hash<K> new_hash{};
	for (size_t i = 0; i < _size; i++)
	{
		if (_mass[i])
		{
			index = new_hash(_mass[i]->_first) % new_size;
			new_mass[index] = new Pair<K, V>(_mass[i]->_first,_mass[i]->_second);
		}
		delete _mass[i];
	}
	delete _mass;
	_mass = new_mass;
	_size = new_size;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::upgrade(const K& key, V&& value)
{
	size_t exist_pair = search_pair(key);
	if (exist_pair != _size)
	{
		_mass[exist_pair]->_second = std::move(value);
		return true;
	}
	return false;
}


template <typename K, typename V, typename H>
bool HashMap<K, V, H>::upgrade(K&& key, V&& value)
{
	size_t exist_pair = search_pair(std::move(key));
	if (exist_pair != _size)
	{
		_mass[exist_pair]->_second = std::move(value);
		return true;
	}
	return false;
}

