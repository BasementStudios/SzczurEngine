#pragma once

#include <any>
#include <map>
#include <string>

#include "Szczur/Portability.hpp"

namespace rat
{

class GlobalVarsHolder
{
public:

	using Key_t    = std::string;
	using Held_t   = std::any;
	using Holder_t = std::map<Key_t, Held_t>;

	GlobalVarsHolder() = default;

	~GlobalVarsHolder() = default;

	GlobalVarsHolder(const GlobalVarsHolder&) = delete;

	GlobalVarsHolder& operator = (const GlobalVarsHolder&) = delete;

	GlobalVarsHolder(GlobalVarsHolder&&) = delete;

	GlobalVarsHolder& operator = (GlobalVarsHolder&&) = delete;

	template <typename T, typename... Ts>
	void create(const Key_t& key, Ts&&... args)
	{
		_holder[key].DEPENDENT_TEMPLATE_SCOPE emplace<T>(std::forward<Ts>(args)...);
	}

	void remove(const Key_t& key)
	{
		_holder.erase(key);
	}

	template <typename T>
	void set(const Key_t& key, T&& arg)
	{
		_holder[key] = std::forward<T>(arg);
	}

	template <typename T>
	T& get(const Key_t& key)
	{
		return std::any_cast<T&>(_holder[key]);
	}

private:

	Holder_t _holder;

};

inline GlobalVarsHolder* gVar;

}
