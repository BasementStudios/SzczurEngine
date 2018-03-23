#pragma once

#include <tuple>

#include "Szczur/Config.hpp"

namespace rat
{

template <typename... Ts>
class Module
{
public:

	using Holder_t = const std::tuple<std::add_lvalue_reference_t<Ts>...>;

	Module();

	Module(const Module&) = default;
	Module& operator = (const Module&) = default;

	Module(Module&&) = default;
	Module& operator = (Module&&) = default;

	~Module() = default;

	template <typename U>
	U& getModule();
	template <typename U>
	const U& getModule() const;

private:

	template <typename U>
	constexpr bool _dependsOn();

	Holder_t _modulesRefs;

};

}

#include "Module.tpp"
