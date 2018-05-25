#pragma once

#include <iterator>

namespace rat
{

///
template <typename ForwardIt, typename T>
constexpr ForwardIt lowerBound(ForwardIt begin, ForwardIt end, T&& value);

///
template <typename ForwardIt, typename T, typename Compare>
constexpr ForwardIt lowerBound(ForwardIt begin, ForwardIt end, T&& value, Compare&& compare);

}


#include "LowerBound.tpp"
