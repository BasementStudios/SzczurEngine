namespace rat
{

template <typename ForwardIt, typename T>
constexpr ForwardIt lowerBound(ForwardIt begin, ForwardIt end, T&& value)
{
	auto count = std::distance(begin, end);

	while (count > 0)
	{
		auto it = begin;
		auto step = count / 2;

		std::advance(it, step);

		if (*it < value)
		{
			begin = ++it;
			count -= step + 1;
		}
		else
		{
			count = step;
		}
	}

	return begin;
}

template <typename ForwardIt, typename T, typename Compare>
constexpr ForwardIt lowerBound(ForwardIt begin, ForwardIt end, T&& value, Compare&& compare)
{
	auto count = std::distance(begin, end);

	while (count > 0)
	{
		auto it = begin;
		auto step = count / 2;

		std::advance(it, step);

		if (compare(*it, value))
		{
			begin = ++it;
			count -= step + 1;
		}
		else
		{
			count = step;
		}
	}

	return begin;
}

}
