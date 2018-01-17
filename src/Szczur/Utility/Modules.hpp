#pragma once

#include <functional>
#include <tuple>

#include "LazyInitializer.hpp"

#include "Szczur/Debug/Logger.hpp"

namespace rat
{

template <typename... Ts>
class Module
{
public:

	template <typename U>
	using Held_t   = std::add_lvalue_reference_t<LazyInitializer<U>>;
	using Holder_t = const std::tuple<Held_t<Ts>...>;

	template <typename U>
	static constexpr bool dependsOn();

	static constexpr size_t dependenciesCount();

	template <typename U>
	Module(U&& tuple);

	Module(const Module&) = delete;

	Module& operator = (const Module&) = delete;

	Module(Module&&) = default;

	Module& operator = (Module&&) = default;

	template <typename U>
	U& getModule();
	template <typename U>
	const U& getModule() const;

private:

	Holder_t _modulesRefs;

};

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
	void initModule(Us&&... args)
	{
		std::get<Held_t<U>>(_modules).init(_modules, std::forward<Us>(args)...);
	}

	template <typename U>
	U& getModule();
	template <typename U>
	const U& getModule() const;

private:

	Holder_t _modules;

};

}

#include "Modules.tpp"
