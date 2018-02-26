#pragma once

#include <tuple>

#include "Module.hpp"
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

	ModulesHolder()
	{
		((modulePtr_v<Ts> = std::get<Held_t<Ts>>(_modules).getPtr()), ...);
	}

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
