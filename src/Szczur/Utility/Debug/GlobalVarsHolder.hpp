#pragma once

#include <any>
#include <map>
#include <string>

namespace rat
{

class GlobalVarsHolder
{
public:

	using Key_t    = std::string;
	using Held_t   = std::any;
	using Holder_t = std::map<Key_t, Held_t>;

	GlobalVarsHolder() = default;

	GlobalVarsHolder(const GlobalVarsHolder&) = delete;
	GlobalVarsHolder& operator = (const GlobalVarsHolder&) = delete;

	GlobalVarsHolder(GlobalVarsHolder&&) = delete;
	GlobalVarsHolder& operator = (GlobalVarsHolder&&) = delete;

	~GlobalVarsHolder() = default;

	template <typename T, typename... Ts>
	void create(const Key_t& key, Ts&&... args);

	void remove(const Key_t& key);

	template <typename T>
	void set(const Key_t& key, T&& arg);

	template <typename T>
	T& get(const Key_t& key);

private:

	Holder_t _holder;

};

inline GlobalVarsHolder* gVar = nullptr;

}

#include "GlobalVarsHolder.tpp"
