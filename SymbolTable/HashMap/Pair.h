#pragma once


template <typename F = int, typename S = std::string>
struct Pair
{
	F _first;
	S _second;

	Pair() : 
		_first(), _second() {}

	Pair(const F &first, const S &second) :
		_first(first), _second(second) {}

	Pair(const Pair<F, S>& pair_el) : 
		_first(pair_el._first), _second(pair_el._second) {}

	Pair(F&& first, S&& second) :
		_first(first), _second(second) {}

	Pair(Pair<F, S>&& pair_el) noexcept :
		_first(std::move(pair_el._first)), _second(std::move(pair_el._second)) {}
	
	Pair<F, S>& operator = (Pair<F, S>&& pair_el) noexcept
	{
		if (this != &pair_el)
		{
			_first = std::move(pair_el._first);
			_second = std::move(pair_el._second);
		}
		return *this;
	}

	~Pair() {}
};