#pragma once

#include <functional>
#include <tuple>

namespace rat
{

template <typename... Ts>
class Module
{
	/* Types */
public:
	struct Updatable {};
	struct Inputable {};
	struct Renderable {};

	using Holder_t = const std::tuple<std::add_lvalue_reference_t<Ts>...>;

	

	/* Operators */
	template <typename U>
	Module(U&& modules);

	// Disable copy constructor and operators 
	Module(const Module&) = delete;
	Module& operator = (const Module&) = delete;

	// Make move constructor and operators default
	Module(Module&&) = default;
	Module& operator = (Module&&) = default;



	/* Methods */
protected:
	template <typename U>
	U& _getModule();
	template <typename U>
	const U& _getModule() const;

public:
	template <typename U>
	static constexpr bool dependsOn();

	static constexpr size_t dependenciesCount();

	

	/* Fields */
private:
	Holder_t _modulesRefs;

};



template <typename... Ts>
class ModulesHolder
{
	/* Types */
public:
	using Holder_t    = std::tuple<Ts...>;
	template <size_t TIndex>
	using NthModule_t = std::tuple_element_t<TIndex, Holder_t>;



	/* Fields */
private:
	Holder_t _modules;



	/* Operators */
public:
	static constexpr size_t modulesCount();

	ModulesHolder();

	// Disable copy and move constructor and operators
	ModulesHolder(const ModulesHolder&) = delete;
	ModulesHolder& operator = (const ModulesHolder&) = delete;

	ModulesHolder(ModulesHolder&&) = delete;
	ModulesHolder& operator = (ModulesHolder&&) = delete;

	template <typename TFunction>
	void forEach(TFunction&& function);
	template <typename TFunction>
	void forEach(TFunction&& function) const;

	template <typename U, typename TFunction>
	void forEach(TFunction&& function);
	template <typename U, typename TFunction>
	void forEach(TFunction&& function) const;

	template <typename U>
	U& getModule();
	template <typename U>
	const U& getModule() const;

private:

	template <typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>);
	template <typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>) const;

	template <typename U, typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>);
	template <typename U, typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>) const;

	template <size_t TIndex, typename U, typename TFunction>
	void _forEachHelper(TFunction&& function);
	template <size_t TIndex, typename U, typename TFunction>
	void _forEachHelper(TFunction&& function) const;
};

}

#include "Modules.tpp"
