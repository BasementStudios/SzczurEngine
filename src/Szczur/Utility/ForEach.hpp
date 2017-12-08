#pragma once

#include <functional>
#include <tuple>

namespace rat
{

namespace detail
{

template<typename T, typename F, size_t... Ns>
inline constexpr void forEach(T&& tuple, F&& function, std::index_sequence<Ns...>);

}

template<typename T, typename F>
inline constexpr void forEach(T&& tuple, F&& function);

}

#include "ForEach.tpp"
