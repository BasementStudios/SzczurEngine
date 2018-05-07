#pragma once

#include <cstdint>

namespace rat
{

using Hash32_t = uint32_t;
using Hash64_t = uint64_t;

///
template <typename InputIt>
constexpr Hash32_t fnv1a_32(InputIt begin, InputIt end);

///
template <typename InputIt>
constexpr Hash32_t fnv1a_32(InputIt begin);

///
template <typename InputIt>
constexpr Hash64_t fnv1a_64(InputIt begin, InputIt end);

///
template <typename InputIt>
constexpr Hash64_t fnv1a_64(InputIt begin);

}

#include "Hash.tpp"
