#pragma once

#include <tuple>

#include "LazyInitializer.hpp"

namespace rat
{

template <typename... Ts>
class ModulesHolder
{
public:

	template <typename U>
	using Held_t   = LazyInitializer<U>;
	using Holder_t = std::tuple<Held_t<Ts>...>;

	template <typename...>
	struct Dummy {};

	static constexpr size_t modulesCount();

	ModulesHolder() = default;

	ModulesHolder(const ModulesHolder&) = delete;

	ModulesHolder& operator = (const ModulesHolder&) = delete;

	ModulesHolder(ModulesHolder&&) = delete;

	ModulesHolder& operator = (ModulesHolder&&) = delete;

	template <typename U, typename... Us>
	void initModule(Us&&... args);

	template <typename U>
	U& getModule();
	template <typename U>
	const U& getModule() const;

private:

	Holder_t _modules;

};

}

#include "ModulesHolder.tpp"
