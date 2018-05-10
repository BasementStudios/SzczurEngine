namespace rat
{

template <typename InputIt>
constexpr Hash32_t fnv1a_32(InputIt begin, InputIt end)
{
	Hash32_t value = 0x811C9DC5u;

	while (begin != end)
	{
		value ^= *begin;
		value *= 0x1000193u;
		++begin;
	}

	return value;
}

template <typename InputIt>
constexpr Hash32_t fnv1a_32(InputIt begin)
{
	Hash32_t value = 0x811C9DC5u;

	while (*begin)
	{
		value ^= *begin;
		value *= 0x1000193u;
		++begin;
	}

	return value;
}

template <typename InputIt>
constexpr Hash64_t fnv1a_64(InputIt begin, InputIt end)
{
	Hash64_t value = 0xCBF29CE484222325ull;

	while (begin != end)
	{
		value ^= *begin;
		value *= 0x100000001B3ull;
		++begin;
	}

	return value;
}

template <typename InputIt>
constexpr Hash64_t fnv1a_64(InputIt begin)
{
	Hash64_t value = 0xCBF29CE484222325ull;

	while (*begin)
	{
		value ^= *begin;
		value *= 0x100000001B3ull;
		++begin;
	}

	return value;
}

}
