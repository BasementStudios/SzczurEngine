#pragma once

#include <tuple>

namespace rat {
	template<typename... TModules>
	class ModuleBase {
	public:

		using Holder_t = const std::tuple<std::add_lvalue_reference_t<TModules>...>;

	private:

		Holder_t _modules;

	public:

		template<typename... UModules>
		ModuleBase(std::tuple<UModules...>& modules);

		ModuleBase(const ModuleBase&) = delete;

		ModuleBase& operator = (const ModuleBase&) = delete;

	protected:

		template<typename TModule>
		TModule& getModule();

		template<typename TModule>
		const TModule& getModule() const;
	};

	template<typename... TModules>
	class ModulesHolder {
	public:

		using Holder_t = std::tuple<TModules...>;

		template<typename...>
		struct Dummy {};

	private:

		std::tuple<TModules...> _modules;

	public:

		ModulesHolder();

		ModulesHolder(const ModulesHolder&) = delete;

		ModulesHolder& operator = (const ModulesHolder&) = delete;

		template<typename TModule>
		TModule& getModule();

		template<typename TModule>
		const TModule& getModule() const;
	};

}

#include "Modules.tpp"
