#pragma once

#include "LowerBound.hpp"

namespace rat
{

///
template <typename ForwardIt, typename T>
constexpr ForwardIt binarySearch(ForwardIt begin, ForwardIt end, T&& value);

///
template <typename ForwardIt, typename T, typename Compare>
constexpr ForwardIt binarySearch(ForwardIt begin, ForwardIt end, T&& value, Compare&& compare);

}

#include "BinarySearch.tpp"
