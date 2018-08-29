#pragma once

#include <tuple>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{

template <typename... Ts>
class Module
{
public:

	using Holder_t = const std::tuple<std::add_lvalue_reference_t<Ts>...>;

	///
	Module();

	///
	Module(const Module&) = delete;

	///
	Module& operator = (const Module&) = delete;

	///
	Module(Module&&) = delete;

	///
	Module& operator = (Module&&) = delete;

	///
	~Module() = default;

	///
	template <typename U>
	U& getModule();

	///
	template <typename U>
	const U& getModule() const;

private:

	Holder_t _modulesRefs;

};

}

#include "Module.tpp"
