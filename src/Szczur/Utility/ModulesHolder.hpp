#pragma once

#include <tuple>

#include "LazyInitializer.hpp"

namespace rat
{

template <typename... Ts>
class ModulesHolder
{
public:

	using Info_t   = std::tuple<LazyInitializer<ModuleVoidAlt_t<Ts>>...>;
	template <typename U>
	using Held_t   = LazyInitializer<ModuleAlt_t<U, Info_t>>;
	using Holder_t = std::tuple<Held_t<Ts>...>;

	template <typename U, typename... Us>
	void initModule(Us&&... args)
	{
		std::get<Held_t<U>>(_holder).init(std::forward<Us>(args)...);
		std::get<Held_t<U>>(_holder).getRef()._holderBegin = reinterpret_cast<Info_t*>(&_holder);
	}

	template <typename U>
	ModuleAlt_t<U, Info_t>& getModule()
	{
		return std::get<Held_t<U>>(_holder).getRef();
	}

	template <typename U>
	const ModuleAlt_t<U, Info_t>& getModule() const
	{
		return std::get<Held_t<U>>(_holder).getRef();
	}

private:

	Holder_t _holder;

};

}
