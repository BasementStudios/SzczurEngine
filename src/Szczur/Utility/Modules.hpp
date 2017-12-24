#pragma once

#include <functional>
#include <tuple>

namespace rat
{

template<typename... Ts>
class Module
{
public:

	using Holder_t = const std::tuple<std::add_lvalue_reference_t<Ts>...>;

	template<typename U>
	static constexpr bool dependsOn();

	static constexpr size_t dependenciesCount();

	template<typename U>
	Module(U&& modules);

	Module(const Module&) = delete;

	Module& operator = (const Module&) = delete;

	Module(Module&&) = default;

	Module& operator = (Module&&) = default;

protected:

	template<typename U>
	U& getModule();
	template<typename U>
	const U& getModule() const;

private:

	Holder_t _modulesRefs;

};

template<typename... Ts>
class ModulesHolder
{
public:

	using Holder_t    = std::tuple<Ts...>;
	template<size_t N>
	using NthModule_t = std::tuple_element_t<N, Holder_t>;

private:

	Holder_t _modules;

public:

	static constexpr size_t modulesCount();

	ModulesHolder();

	ModulesHolder(const ModulesHolder&) = delete;

	ModulesHolder& operator = (const ModulesHolder&) = delete;

	ModulesHolder(ModulesHolder&&) = delete;

	ModulesHolder& operator = (ModulesHolder&&) = delete;

	template<typename F>
	void forEach(F&& function);
	template<typename F>
	void forEach(F&& function) const;

	template<typename U, typename F>
	void forEach(F&& function);
	template<typename U, typename F>
	void forEach(F&& function) const;

	template<typename U>
	U& getModule();
	template<typename U>
	const U& getModule() const;

private:

	template<typename F, size_t... Ns>
	void _forEach(F&& function, std::index_sequence<Ns...>);
	template<typename F, size_t... Ns>
	void _forEach(F&& function, std::index_sequence<Ns...>) const;

	template<typename U, typename F, size_t... Ns>
	void _forEach(F&& function, std::index_sequence<Ns...>);
	template<typename U, typename F, size_t... Ns>
	void _forEach(F&& function, std::index_sequence<Ns...>) const;

	template<size_t N, typename U, typename F>
	void _forEachHelper(F&& function);
	template<size_t N, typename U, typename F>
	void _forEachHelper(F&& function) const;

};

struct Inputable {};
struct Updatable {};
struct Renderable {};

}

#include "Modules.tpp"
