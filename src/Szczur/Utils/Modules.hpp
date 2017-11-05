#pragma once

#include <tuple>

namespace rat {
	template<typename... Ts>
	class ModuleBase {
	public:

		using Holder_t = const std::tuple<std::add_lvalue_reference_t<Ts>...>;

	private:

		Holder_t _modules;

	public:

		template<typename... Us>
		ModuleBase(std::tuple<Us...>& modules);

		ModuleBase(const ModuleBase&) = delete;

		ModuleBase& operator = (const ModuleBase&) = delete;

	protected:

		template <typename T>
		T& getModule();

		template <typename T>
		const T& getModule() const;
	};

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

#include "Modules.tpp"

}
