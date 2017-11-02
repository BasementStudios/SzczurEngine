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

#include "ModuleBase.tpp"

}
