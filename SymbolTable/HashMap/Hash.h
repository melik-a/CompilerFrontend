#pragma once

#include <string>
#include <iostream>


template <typename K>
struct Hash
{
	size_t operator()(const K& key)
	{
		size_t hash = 0;
		try
		{
			hash = (size_t) key & 0x07FFFFFFF;
			hash = hash * (PRIME_1 ^ PRIME_3) * (PRIME_2 ^ PRIME_4);
		}
		catch (...)
		{
			std::cout << "this type can't be hashed -> error code (-1) is returning\n";
			return 0;
		}
		return hash;
	}

	size_t operator()(K&& key)
	{
		size_t hash = 0;
		try
		{
			hash = (size_t)key & 0x07FFFFFFF;
			hash = hash * (PRIME_1 ^ PRIME_3) * (PRIME_2 ^ PRIME_4);
		}
		catch (...)
		{
			std::cout << "this type can't be hashed -> error code (-1) is returning\n";
			return 0;
		}
		return hash;
	}

	private:
		static constexpr size_t PRIME_1 = 12289;
		static constexpr size_t PRIME_2 = 18181;
		static constexpr size_t PRIME_3 = 1354828;
		static constexpr size_t PRIME_4 = 524287;
};


template <>
struct Hash<std::string> 
{
	size_t operator()(const std::string& key)
	{
		size_t hash = 0;
		int i = 1;
		for (char c : key)
		{
			hash += (size_t)pow(c, i) * (PRIME_1 ^ PRIME_3) * (PRIME_2 ^ PRIME_4);
			i++;
		}
		return hash;
	}

	size_t operator()(std::string&& key)
	{
		size_t hash = 0;
		int i = 1;
		for (char c : key)
		{
			hash += (size_t)pow(c, i) * (PRIME_1 ^ PRIME_3) * (PRIME_2 ^ PRIME_4);
			i++;
		}
		return hash;
	}

	private:
		static constexpr size_t PRIME_1 = 12289;
		static constexpr size_t PRIME_2 = 18181;
		static constexpr size_t PRIME_3 = 1354828;
		static constexpr size_t PRIME_4 = 524287;
};

