#pragma once

#include <cstddef>
#include <tuple>

#include "LazyInitializer.hpp"

namespace rat
{

template <typename, typename>
struct ModuleAlt;

template <typename T, typename U>
using ModuleAlt_t = typename ModuleAlt<T, U>::Alt_t;

template <typename T>
using ModuleVoidAlt_t = typename ModuleAlt<T, void>::Alt_t;

#define REGISTER_GAME_MODULE(_Name, _Class) template <typename T> struct ModuleAlt<_Name, T> { using Alt_t = _Class<T>; }

template <typename Info>
class Module
{
template <typename... Us>
friend class ModulesHolder;

public:

	using Info_t = Info;

	template <typename Module>
	ModuleAlt_t<Module, Info>& getModule()
	{
		return reinterpret_cast<LazyInitializer<ModuleAlt_t<Module, Info>>&>(std::get<LazyInitializer<ModuleVoidAlt_t<Module>>>(*_holderBegin)).getRef();
	}

	template <typename Module>
	const ModuleAlt_t<Module, Info>& getModule() const
	{
		return reinterpret_cast<LazyInitializer<ModuleAlt_t<Module, Info>>&>(std::get<LazyInitializer<ModuleVoidAlt_t<Module>>>(*_holderBegin)).getRef();
	}

private:

	Info_t* _holderBegin;

};

}
