#pragma once

#include <tuple>

#include "Szczur/Debug/Logger.hpp"
#include "LazyInitializer.hpp"

namespace rat
{

template <typename... Ts>
class Module
{
public:

	using Holder_t = const std::tuple<std::add_lvalue_reference_t<Ts>...>;

	template <typename U>
	static constexpr bool dependsOn();

	static constexpr size_t dependenciesCount();

	template <typename U>
	Module(U&& tuple);

	Module(const Module&) = default;

	Module& operator = (const Module&) = default;

	Module(Module&&) = default;

	Module& operator = (Module&&) = default;

	template <typename U>
	U& getModule();
	template <typename U>
	const U& getModule() const;

private:

	Holder_t _modulesRefs;

};

}

#include "Module.tpp"
