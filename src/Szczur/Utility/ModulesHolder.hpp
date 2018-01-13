#pragma once

#include "AlignedUnion.hpp"
#include "LazyInitializer.hpp"
#include "VariadicPackInfo.hpp"

namespace rat
{

template <typename... Ts>
class ModulesHolder
{
public:

	using Info_t   = VariadicPackInfo<ModuleVoidAlt_t<Ts>...>;
	template <typename U>
	using Held_t   = LazyInitializer<ModuleAlt_t<U, Info_t>>;
	using Holder_t = AlignedUnion<Held_t<Ts>...>;

	template <typename U, typename... Us>
	void initModule(Us&&... args)
	{
		_getInitializer<U>().init(std::forward<Us>(args)...);
	}

	template <typename U>
	ModuleAlt_t<U, Info_t>& getModule()
	{
		return _getInitializer<U>().getRef();
	}

	template <typename U>
	const ModuleAlt_t<U, Info_t>& getModule() const
	{
		return _getInitializer<U>().getRef();
	}

private:

	template <typename U>
	Held_t<U>& _getInitializer()
	{
		return *_holder.template getMemoryOf<Held_t<U>>();
	}

	template <typename U>
	const Held_t<U>& _getInitializer() const
	{
		return *_holder.template getMemoryOf<Held_t<U>>();
	}

	Holder_t _holder;

};

}
