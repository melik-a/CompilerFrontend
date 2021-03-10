#pragma once
#include "../../Parser/include/SyntaxTag.h"
#include <string>

template <typename F = int, typename S = std::string, typename T = SyntaxTag>
struct Identifier
{
	F _key;
	S _value;
	T _datatype;
	Identifier() : 
		_key(), _value(), _datatype(SyntaxTag::FLOAT_NUMBER) {}

	Identifier(const F& key, const S& value, const T& type) :
		_key(key), _value(value), _datatype(type) {}

	Identifier(const Identifier<F, S, T>& id_el) : 
		_key(id_el._key), _value(id_el._value), _datatype(id_el._datatype) {}

	Identifier(F&& key, S&& value, T&& type) :
		_key(key), _value(value), _datatype(type) {}

	Identifier(Identifier<F, S, T>&& id_el) noexcept :
		_key(std::move(id_el._key)), _value(std::move(id_el._value)) {}
	
	Identifier<F, S, T>& operator = (Identifier<F, S, T>&& id_el) noexcept
	{
		if (this != &id_el)
		{
			_key = std::move(id_el._key);
			_value = std::move(id_el._value);
			_datatype = std::move(id_el._datatype);
		}
		return *this;
	}

	~Identifier() {}
};