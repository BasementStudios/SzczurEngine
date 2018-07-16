namespace rat
{

template <typename ForwardIt, typename T>
constexpr ForwardIt binarySearch(ForwardIt begin, ForwardIt end, T&& value)
{
	begin = lowerBound(begin, end, value);

	return begin != end && !(value < *begin) ? begin : end;
}

template <typename ForwardIt, typename T, typename Compare>
constexpr ForwardIt binarySearch(ForwardIt begin, ForwardIt end, T&& value, Compare&& compare)
{
	begin = lowerBound(begin, end, value, compare);

	return begin != end && !compare(value, *begin) ? begin : end;
}

}
