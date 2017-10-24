#pragma once

#include <Szczur/Core/ModuleBase.h>

template<typename... Ts>
class ModulesHolder {
public:

	using Holder_t  = std::tuple<Ts...>;
	using Swallow_t = int[];

	template<typename...>
	struct Dummy {};

private:

	std::tuple<Ts...> _modules;

public:

	ModulesHolder() :
		_modules(((void)Dummy<Ts>{}, _modules)...) {}

	ModulesHolder(const ModulesHolder&) = delete;

	ModulesHolder& operator = (const ModulesHolder&) = delete;

	template <typename F>
	void forEach(F&& function) {
		forEachImpl(std::forward<F>(function), std::index_sequence_for<Ts...>{});
	}

	template <typename T>
	T& get() {
		return std::get<T>(_modules);
	}

	template <typename T>
	const T& get() const {
		return std::get<T>(_modules);
	}

private:

	template<typename F, size_t... Is>
	void forEachImpl(F&& function, std::index_sequence<Is...>) {
		(void)Swallow_t{ (function(std::get<Is>(_modules)), 0)... };
	}

};
