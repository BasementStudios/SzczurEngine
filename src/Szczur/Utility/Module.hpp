#pragma once

#include <cstddef>

namespace rat
{

template <typename, typename>
struct ModuleAlt;

template <typename T, typename U>
using ModuleAlt_t = typename ModuleAlt<T, U>::Alt_t;

template <typename T>
using ModuleVoidAlt_t = typename ModuleAlt<T, void>::Alt_t;

#define REGISTER_MOD3_MODULE(_Name, _Class) template <typename T> struct ModuleAlt<_Name, T> { using Alt_t = _Class<T>; }

template <typename Super, typename Info>
class Module
{
public:

	template <typename Module>
	ModuleAlt_t<Module, Info>& getModule()
	{
		using Pointer_t = std::byte*;

		Pointer_t ptr = reinterpret_cast<Pointer_t>(this);
		ptr -= Info::template offsetOf<ModuleVoidAlt_t<Super>>();
		ptr += Info::template offsetOf<ModuleVoidAlt_t<Module>>();

		return *reinterpret_cast<ModuleAlt_t<Module, Info>*>(ptr);
	}

	template <typename Module>
	const ModuleAlt_t<Module, Info>& getModule() const
	{
		using ConstPointer_t = const std::byte*;

		ConstPointer_t ptr = reinterpret_cast<ConstPointer_t>(this);
		ptr -= Info::template offsetOf<ModuleVoidAlt_t<Super>>();
		ptr += Info::template offsetOf<ModuleVoidAlt_t<Module>>();

		return *reinterpret_cast<ModuleAlt_t<Module, Info>*>(ptr);
	}

	size_t getModulesCount()
	{
		return Info::typesCount();
	}

};

}
