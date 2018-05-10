#pragma once

#include <cstddef>

namespace rat
{

namespace detail
{

template <typename T>
inline static size_t globalID_v = 0u;

}

template <typename T>
size_t setInitialUniqueID(size_t id)
{
    detail::globalID_v<T> = id;
}

template <typename T>
size_t getUniqueID()
{
    return ++detail::globalID_v<T>;
}

}
