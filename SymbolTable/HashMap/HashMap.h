#pragma once

#include <iostream>
#include "Hash.h"
#include "Identifier.h"
#include <cassert>

/***  
 *** simple implementation of hash table
 *** template arguments: K - type of storage key,
 ***					 V - type of storage value,
 ***					 D - type of datatype of id,
 ***					 H - hash function (by dedault used Hash<K>{})
 *** 
 *** implementation provides some general methods: insert, remove, search, update, resize and etc.
 *** 
 *** to use this hash table implementaion with some non-trivial structures
 *** need to transfer as a template argument some hash functions that can provide hashing this structure.
 *** 
 *** also hash table supports move semantics.
 *** 
 ***/

template <typename K, typename V, typename D = SyntaxTag, typename H = Hash<K>>
struct HashMap
{
	HashMap() : 
		_size(_default_size), _mass(new Identifier<K, V, D>* [_size] { nullptr }) {}
	
	HashMap(size_t size) :
		_size(size), _mass(new Identifier<K, V, D>* [_size] { nullptr }) {}

	~HashMap()
	{
		if (_mass)
		{
			for (size_t i = 0; i < _size; i++)
				delete _mass[i];
			delete _mass;
		}
	}
	
	bool is_empty() const;
	bool is_full() const;
	double load_factor() const;
	double load_factor_threshold() const;
	size_t default_size() const;

	void print_all_entries() const;
	void print_all_keys() const;
	void print_all_values() const;

	Identifier<K, V, D>* const* get_all_id() const;

	bool insert(const K& key, const V& value, const D& type);
	bool insert(K&& key, V&& value, D&& type);
	bool insert(Identifier<K, V, D>&& new_pair);
	bool insert(const Identifier<K, V, D>& new_pair);

	size_t search(const K& key);
	size_t search(K&& key);

	bool remove(const K& key);
	bool remove(K&& key);

	bool update(const K& key, V&& value);
	bool update(K&& key, V&& value);

	size_t get_number_of_entries() const;
	size_t get_size() const;
	size_t get_last_number_of_comparisons() const;

	const Identifier<K, V, D>* operator[](const size_t& index) const;
	const Identifier<K, V, D>* operator[](const K& key);
	const Identifier<K, V, D>* operator[](K&& key);

	private:
		static constexpr size_t _default_size = 200;
		static constexpr float _load_factor_threshold = 0.6f;
		static constexpr short _growth_factor = 2;

		size_t _size;
		Identifier<K, V, D> **_mass;
		H _hash{};
		size_t _number_of_entries{};
		size_t _last_num_comparisons{};

		void resize_table();
};


template <typename K, typename V, typename D, typename H>
std::ostream& operator << (std::ostream& os, const HashMap<K, V, D, H>& table)
{
	os << "\n{ ";
	size_t size = table.get_size();
	for (size_t i = 0; i < size; i++)
	{
		if (table[i])
			os << table[i]->_key << " : " << table[i]->_value << " : " << table[i]->_datatype;
		else
			os << " : : ";
		
		if (i < size - 1)
			std::cout << ", ";
	}
	os << " }\n";
	return os;
}


template <typename K, typename V, typename D, typename H>
const Identifier<K, V, D>* HashMap<K, V, D, H>::operator[](const size_t& index) const
{
	assert(index < _size);
	return _mass[index];
}


template <typename K, typename V, typename D, typename H>
const Identifier<K, V, D>* HashMap<K, V, D, H>::operator[](const K& key)
{
	size_t index = search(key);
	return index == get_size() ? nullptr : _mass[index];
}


template <typename K, typename V, typename D, typename H>
const Identifier<K, V, D>* HashMap<K, V, D, H>::operator[](K&& key)
{
	size_t index = this->search(std::move(key));
	return index == get_size() ? nullptr : _mass[index];
}


template <typename K, typename V, typename D, typename H>
size_t HashMap<K, V, D, H>::get_number_of_entries() const
{
	return _number_of_entries;
}


template <typename K, typename V, typename D, typename H>
size_t HashMap<K, V, D, H>::get_size() const
{
	return _size;
}


template <typename K, typename V, typename D, typename H>
size_t HashMap<K, V, D, H>::get_last_number_of_comparisons() const
{
	return _last_num_comparisons;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::is_empty() const
{
	return _number_of_entries ? false : true;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::is_full() const
{
	return _number_of_entries == _size ? true : false;
}


template <typename K, typename V, typename D, typename H>
double HashMap<K, V, D, H>::load_factor() const
{
	return (double)_number_of_entries / _size;
}


template <typename K, typename V, typename D, typename H>
double HashMap<K, V, D, H>::load_factor_threshold() const
{
	return _load_factor_threshold;
}


template <typename K, typename V, typename D, typename H>
size_t HashMap<K, V, D, H>::default_size() const
{
	return _default_size;
}


template <typename K, typename V, typename D, typename H>
void HashMap<K, V, D, H>::print_all_entries() const
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
			std::cout << "{ " << _mass[i]->_key << " : " << _mass[i]->_value
												<< " : " << _mass[i]->_value << " }";
			if (i < _size - 1)
				std::cout << " ";
		}
	}
	std::cout << "}" << std::endl;
}


template <typename K, typename V, typename D, typename H>
void HashMap<K, V, D, H>::print_all_keys() const
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
			std::cout << _mass[i]->_key;
		else
			std::cout << "nullptr";

		if (i < _size - 1)
			std::cout << ", ";
	}
	std::cout << "}" << std::endl;
}


template <typename K, typename V, typename D, typename H>
void HashMap<K, V, D, H>::print_all_values() const
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
			std::cout << _mass[i]->_value;
		else
			std::cout << "nullptr";
		
		if (i < _size - 1)
			std::cout << ", ";
	}
	std::cout << "}" << std::endl;
}


template <typename K, typename V, typename D, typename H>
Identifier<K, V, D>* const* HashMap<K, V, D, H>::get_all_id() const
{
	return _mass;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::insert(const K& key, const V& value, const D& type)
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
		_mass[index] = new Identifier<K, V, D> (key, value, type);
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::insert(K&& key, V&& value, D&& type)
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
		_mass[index] = new Identifier<K, V, D>(std::move(key), std::move(value), std::move(type));
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::insert(Identifier<K, V, D>&& new_id)
{
	if (_load_factor_threshold < load_factor())
		resize_table();

	_last_num_comparisons = 0;

	size_t index = _hash(std::move(new_id._key)) % _size;
	for (size_t i = 1; (i < _size) && (_mass[index]); i++)
	{
		index = (_hash(std::move(new_id._key)) + i) % _size;
		_last_num_comparisons++;
	}

	_last_num_comparisons++;
	if (!_mass[index])
	{
		_mass[index] = new Identifier<K, V, D>(std::move(new_id));		
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::insert(const Identifier<K, V, D>& new_id)
{
	if (_load_factor_threshold < load_factor())
		resize_table();

	_last_num_comparisons = 0;

	size_t index = _hash(new_id._key) % _size;
	for (size_t i = 1; (i < _size) && (_mass[index]); i++)
	{
		index = (_hash(new_id._key) + i) % _size;
		_last_num_comparisons++;
	}

	_last_num_comparisons++;
	if (!_mass[index])
	{
		_mass[index] = new Identifier<K, V, D>(new_id);
		_number_of_entries++;
		return true;
	}
	return false;
}


template <typename K, typename V, typename D, typename H>
size_t HashMap<K, V, D, H>::search(const K& key)
{
	_last_num_comparisons = 0;

	size_t index = _hash(key) % _size;

	// if elem is empty -> didn't find elem, else check keys on equal
	bool find = (!_mass[index] ? false : _mass[index]->_key == key);
	for (size_t i = 1; (i < _size) && (!find); i++)
	{
		index = (_hash(key) + i) % _size;
		_last_num_comparisons++;
		find = (!_mass[index] ? false : _mass[index]->_key == key);
	}

	_last_num_comparisons++;
	if (find)
		return index;
	return _size;
}


template <typename K, typename V, typename D, typename H>
size_t HashMap<K, V, D, H>::search(K&& key)
{
	_last_num_comparisons = 0;

	size_t index = _hash(std::move(key)) % _size;

	// if elem is empty -> didn't find elem, else check keys on equal
	bool find = (!_mass[index] ? false : _mass[index]->_key == key);
	for (size_t i = 1; (i < _size) && (!find); i++)
	{
		index = (_hash(std::move(key)) + i) % _size;
		_last_num_comparisons++;
		find = (!_mass[index] ? false : _mass[index]->_key == key);
	}

	_last_num_comparisons++;
	if (find)
		return index;
	return _size;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::remove(const K& key)
{
	size_t index = search(key);
	if (index != _size)
	{
		_number_of_entries--;
		_mass[index] = nullptr;
		return true;
	}
	return false;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::remove(K&& key)
{
	size_t index = search(std::move(key));
	if (index != _size)
	{
		_number_of_entries--;
		_mass[index] = nullptr;
		return true;
	}
	return false;
}


template <typename K, typename V, typename D, typename H>
void HashMap<K, V, D, H>::resize_table()
{
	size_t new_size = _size * _growth_factor;
	Identifier<K, V, D> **new_mass = new Identifier<K, V, D>*[new_size] { nullptr };
	size_t index{};
	Hash<K> new_hash{};
	for (size_t i = 0; i < _size; i++)
	{
		if (_mass[i])
		{
			index = new_hash(_mass[i]->_key) % new_size;
			new_mass[index] = new Identifier<K, V, D>(_mass[i]->_key,_mass[i]->_value, _mass[i]->_datatype);
		}
		delete _mass[i];
	}
	delete _mass;
	_mass = new_mass;
	_size = new_size;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::update(const K& key, V&& value)
{
	size_t exist_pair = search(key);
	if (exist_pair != _size)
	{
		_mass[exist_pair]->_value = std::move(value);
		return true;
	}
	return false;
}


template <typename K, typename V, typename D, typename H>
bool HashMap<K, V, D, H>::update(K&& key, V&& value)
{
	size_t exist_pair = search(std::move(key));
	if (exist_pair != _size)
	{
		_mass[exist_pair]->_value = std::move(value);
		return true;
	}
	return false;
}

