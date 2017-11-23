#pragma once

#include <tuple>

#include <SFML/Window/Event.hpp>

#include "ForEach.hpp"

namespace rat {
	template<typename... TModules>
	class ModuleBase {
	public:

		using Holder_t = const std::tuple<std::add_lvalue_reference_t<TModules>...>;

	private:

		Holder_t _modules;

	public:

		template<typename UTuple>
		ModuleBase(UTuple&& modules);

		ModuleBase(const ModuleBase&) = default;

		ModuleBase& operator = (const ModuleBase&) = default;

		ModuleBase(ModuleBase&&) = default;

		ModuleBase& operator = (ModuleBase&&) = default;

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

		ModulesHolder(ModulesHolder&&) = delete;

		ModulesHolder& operator = (ModulesHolder&&) = delete;

		template<typename F>
		void forEach(F&& function);
		template<typename F>
		void forEach(F&& function) const;

		template<typename TModule>
		TModule& getModule();

		template<typename TModule>
		const TModule& getModule() const;
	};

}

#include "Modules.tpp"
