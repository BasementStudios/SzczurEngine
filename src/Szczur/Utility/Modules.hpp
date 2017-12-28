#pragma once

#include <functional>
#include <tuple>

namespace rat
{

template<typename... TModules>
class Module
{
public:

	using Holder_t = const std::tuple<std::add_lvalue_reference_t<TModules>...>;

	template<typename TModule>
	static constexpr bool dependsOn();

	static constexpr size_t dependenciesCount();

	template<typename TModule>
	Module(TModule&& modules);

	Module(const Module&) = delete;

	Module& operator = (const Module&) = delete;

	Module(Module&&) = default;

	Module& operator = (Module&&) = default;

protected:

	template<typename TModule>
	TModule& _getModule();
	template<typename TModule>
	const TModule& _getModule() const;

private:

	Holder_t _modulesRefs;

};

template<typename... TModules>
class ModulesHolder
{
public:

	using Holder_t    = std::tuple<TModules...>;
	template<size_t TIndex>
	using NthModule_t = std::tuple_element_t<TIndex, Holder_t>;

private:

	Holder_t _modules;

public:

	static constexpr size_t modulesCount();

	ModulesHolder();

	ModulesHolder(const ModulesHolder&) = delete;

	ModulesHolder& operator = (const ModulesHolder&) = delete;

	ModulesHolder(ModulesHolder&&) = delete;

	ModulesHolder& operator = (ModulesHolder&&) = delete;

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

struct Inputable {};
struct Updatable {};
struct Renderable {};

}

#include "Modules.tpp"
