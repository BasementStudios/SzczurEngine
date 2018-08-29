#pragma once

#include <cstdint>
#include <cstddef>
#include <utility>

namespace rat::world
{

using Name_t     = std::string;
using Registry_t = entt::DefaultRegistry;
using EntityID_t = Registry_t::entity_type;
using Hash_t     = uint64_t;

namespace detail
{

/// Computes 64-bit alternative Fowler–Noll–Vo hash from null-terminated source
template <typename InputIt>
constexpr Hash_t fnv1a_64(InputIt begin)
{
	Hash_t value = 0xCBF29CE484222325ull;

	while (*begin)
	{
		value ^= *begin;
		value *= 0x100000001B3ull;
		++begin;
	}

	return value;
}

/// Computes 64-bit alternative Fowler–Noll–Vo hash from source range
template <typename InputIt>
constexpr Hash_t fnv1a_64(InputIt begin, InputIt end)
{
	Hash_t value = 0xCBF29CE484222325ull;

	while (begin != end)
	{
		value ^= *begin;
		value *= 0x100000001B3ull;
		++begin;
	}

	return value;
}

}

class HashedID
{
public:

	///
	template <size_t N>
	HashedID(const char (&arr)[N])
		: hash { detail::fnv1a_64(arr, arr + N) }
		#ifdef DEBUG
		, str { arr, N }
		#endif
	{

	}

	///
	HashedID(const char* src)
		: hash { detail::fnv1a_64(src) }
		#ifdef DEBUG
		, str { src }
		#endif
	{

	}

	///
	HashedID(Name_t str)
		: hash { detail::fnv1a_64(str.begin(), str.end()) }
		#ifdef DEBUG
		, str { std::move(str) }
		#endif
	{

	}

	///
	HashedID(const HashedID&) = default;

	///
	HashedID& operator = (const HashedID&) = default;

	///
	HashedID(HashedID&&) = default;

	///
	HashedID& operator = (HashedID&&) = default;

	const Hash_t hash;
	#ifdef DEBUG
	const Name_t str;
	#endif

};


}
