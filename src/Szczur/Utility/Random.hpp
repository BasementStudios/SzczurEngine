#pragma once

#include <random>

namespace rat
{

class Random
{
private:
	std::mt19937 _gen;

public:
	Random()
	{
		_gen = std::mt19937({ std::random_device{}() });
	}

	Random(unsigned seed)
		: _gen{ seed }
	{
	}

	~Random() = default;

	template<typename T>
	T get(T min, T max)
	{
		using Dist_t = std::conditional_t<std::is_integral_v<T>, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;

		return Dist_t{ min, max }(_gen);
	}
};

}
