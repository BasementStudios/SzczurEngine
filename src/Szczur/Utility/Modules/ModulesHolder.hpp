#pragma once

#include <algorithm>
#include <tuple>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

namespace detail
{

template <size_t N, typename T, typename U, typename... Us>
struct IndexOffset : IndexOffset<N + 1u, T, Us...> {};

template <size_t N, typename T, typename... Us>
struct IndexOffset<N, T, T, Us...> : std::integral_constant<size_t, N> {};

template <size_t N, typename T, typename U, typename... Us>
struct SizeOffset : SizeOffset<N + sizeof(U), T, Us...> {};

template <size_t N, typename T, typename... Us>
struct SizeOffset<N, T, T, Us...> : std::integral_constant<size_t, N> {};

template <size_t N, typename T, typename... Ts>
struct NthElement : NthElement<N - 1u, Ts...> {};

template <typename T, typename... Ts>
struct NthElement<0u, T, Ts...> { using type = T; };

template <size_t N, typename... Ts>
using NthElement_t = typename NthElement<N, Ts...>::type;

template <typename... Ts>
constexpr size_t StorageSizeFor()
{
	size_t size = 0u;

	using Swallow_t = int[];
	Swallow_t{ ((size += sizeof(Ts)), 0)... };

	return size;
}

template <typename... Ts>
constexpr size_t StorageAlignFor()
{
	size_t align = 0u;

	using Swallow_t = int[];
	Swallow_t{ ((align = alignof(Ts) > align ? alignof(Ts) : align), 0)... };

	return align;
}

}

template <typename... Ts>
class ModulesHolder
{
public:

	using Holder_t = std::aligned_storage_t<detail::StorageSizeFor<Ts...>(), detail::StorageAlignFor<Ts...>()>;

	///
	ModulesHolder();

	///
	ModulesHolder(const ModulesHolder&) = delete;

	///
	ModulesHolder& operator = (const ModulesHolder&) = delete;

	///
	ModulesHolder(ModulesHolder&&) = delete;

	///
	ModulesHolder& operator = (ModulesHolder&&) = delete;

	///
	~ModulesHolder();

	///
	template <typename U, typename... Us>
	void initModule(Us&&... args);

	///
	template <typename U>
	U& getModule();

	///
	template <typename U>
	const U& getModule() const;

	///
	template <size_t N>
	detail::NthElement_t<N, Ts...>& getModule();

	///
	template <size_t N>
	const detail::NthElement_t<N, Ts...>& getModule() const;

private:

	///
	template <typename U>
	void* getStoragePtrFor();

	///
	template <typename U>
	const void* getStoragePtrFor() const;

	///
	template <size_t... Ns>
	void _destroyAll(std::index_sequence<Ns...>);

	///
	template <typename U>
	void _destroy();

	Holder_t _holder;

};

}

#include "ModulesHolder.tpp"
