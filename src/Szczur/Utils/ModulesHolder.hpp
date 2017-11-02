#pragma once

#include "ModuleBase.hpp"

namespace rat {
	template<typename... Ts>
	class ModulesHolder {
	public:

		using Holder_t  = std::tuple<Ts...>;

		template<typename...>
		struct Dummy {};

	private:

		std::tuple<Ts...> _modules;

	public:

		ModulesHolder();

		ModulesHolder(const ModulesHolder&) = delete;

		ModulesHolder& operator = (const ModulesHolder&) = delete;

		template <typename T>
		T& get();

		template <typename T>
		const T& get() const;
	};

#include "ModulesHolder.tpp"

}
