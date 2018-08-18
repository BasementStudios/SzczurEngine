#pragma once

#include <cstdint>
#include <cstddef>
#include <utility>

namespace rat::wrd
{

using ID_t       = size_t;
using Name_t     = std::string;
using NameView_t = std::string_view;
using Registry_t = entt::DefaultRegistry;
using EntityID_t = entt::DefaultRegistry::entity_type;
using Hash_t     = uint64_t;

namespace detail
{

///
constexpr Hash_t fnv1a_64(const char* src)
{
    Hash_t value = 0xCBF29CE484222325ull;

    while (*src)
    {
        value ^= *src;
        value *= 0x100000001B3ull;
        ++src;
    }

    return value;
}

}

struct HashedID
{
    ///
    HashedID(Hash_t value) : hash { value } {}

    ///
    HashedID(const char* src) : hash { detail::fnv1a_64(src) } {}

    const Hash_t hash;

};


}
