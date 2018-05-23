#pragma once

#include <cstddef>

namespace rat
{

namespace detail
{

template <typename T>
inline static size_t globalID = 0u;

}

template <typename T>
size_t setInitialUniqueID(size_t id)
{
    detail::globalID<T> = id;
}

template <typename T>
void trySettingInitialUniqueID(size_t id)
{
    if (id > detail::globalID<T>)
    {
        detail::globalID<T> = id;
    }
}

template <typename T>
size_t getUniqueID()
{
    return ++detail::globalID<T>;
}

}
