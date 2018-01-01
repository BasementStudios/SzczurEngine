#pragma once

#include <functional>
#include <tuple>

namespace rat
{

/** @class Module
 ** @description Module class.
 **/
template<typename... TModules>
class Module
{
	/* Types */
public:
	// Traits
	struct Inputable {};
	struct Updatable {};
	struct Renderable {};
	
	using Holder_t = const std::tuple<std::add_lvalue_reference_t<TModules>...>;



	/* Variables */
private:
	Holder_t _modulesRefs;



	/* Operators */
public:
	template<typename TModule>
	Module(TModule&& modules);

	// Disable copy and move constructor and operators 
	Module(const Module&) = delete;
	Module& operator = (const Module&) = delete;

	Module(Module&&) = default;
	Module& operator = (Module&&) = default;



	/* Methods */
public:
	template<typename TModule>
	static constexpr bool dependsOn();

	static constexpr size_t dependenciesCount();

protected:
	template<typename TModule>
	TModule& _getModule();
	template<typename TModule>
	const TModule& _getModule() const;
};



/** @class ModulesHolder
 ** @description Module holder class.
 **/
template<typename... TModules>
class ModulesHolder
{
	/* Types */
public:
	using Holder_t    = std::tuple<TModules...>;
	template<size_t TIndex>
	using NthModule_t = std::tuple_element_t<TIndex, Holder_t>;



	/* Variables */
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



	/* Methods */
public:
	template<typename TFunction>
	void forEach(TFunction&& function);
	template<typename TFunction>
	void forEach(TFunction&& function) const;

	template<typename TModule, typename TFunction>
	void forEach(TFunction&& function);
	template<typename TModule, typename TFunction>
	void forEach(TFunction&& function) const;

	template<typename TModule>
	TModule& getModule();
	template<typename TModule>
	const TModule& getModule() const;

private:
	template<typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>);
	template<typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>) const;

	template<typename TModule, typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>);
	template<typename TModule, typename TFunction, size_t... TIndices>
	void _forEach(TFunction&& function, std::index_sequence<TIndices...>) const;

	template<size_t TIndex, typename TModule, typename TFunction>
	void _forEachHelper(TFunction&& function);
	template<size_t TIndex, typename TModule, typename TFunction>
	void _forEachHelper(TFunction&& function) const;
};

}

#include "Modules.tpp"
