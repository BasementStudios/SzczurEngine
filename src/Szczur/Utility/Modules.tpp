namespace rat
{

template <typename... Ts>
template <typename U>
constexpr bool Module<Ts...>::dependsOn()
{
	return (std::is_same_v<U, Ts> || ...);
}

template <typename... Ts>
constexpr size_t Module<Ts...>::dependenciesCount()
{
	return sizeof...(Ts);
}

template <typename... Ts>
template <typename U>
Module<Ts...>::Module(U&& modules) :
	_modulesRefs(std::get<Ts>(modules)...)
{

}

template <typename... Ts>
template <typename U>
U& Module<Ts...>::_getModule()
{
	static_assert(dependsOn<U>());

	return std::get<U&>(_modulesRefs);
}

template <typename... Ts>
template <typename U>
const U& Module<Ts...>::_getModule() const
{
	static_assert(dependsOn<U>());

	return std::get<U&>(_modulesRefs);
}

template <typename... Ts>
constexpr size_t ModulesHolder<Ts...>::modulesCount()
{
	return std::tuple_size_v<std::decay_t<Holder_t>>;
}

template <typename... Ts>
ModulesHolder<Ts...>::ModulesHolder() :
	_modules(Ts{ _modules }...)
{

}

template <typename... Ts>
template <typename F>
void ModulesHolder<Ts...>::forEach(F&& function)
{
	_forEach(std::forward<F>(function), std::make_index_sequence<modulesCount()>{});
}

template <typename... Ts>
template <typename F>
void ModulesHolder<Ts...>::forEach(F&& function) const
{
	_forEach(std::forward<F>(function), std::make_index_sequence<modulesCount()>{});
}

template <typename... Ts>
template <typename U, typename F>
void ModulesHolder<Ts...>::forEach(F&& function)
{
	_forEach<U>(std::forward<F>(function), std::make_index_sequence<modulesCount()>{});
}

template <typename... Ts>
template <typename U, typename F>
void ModulesHolder<Ts...>::forEach(F&& function) const
{
	_forEach<U>(std::forward<F>(function), std::make_index_sequence<modulesCount()>{});
}

template <typename... Ts>
template <typename U>
U& ModulesHolder<Ts...>::getModule()
{
	return std::get<U>(_modules);
}

template <typename... Ts>
template <typename U>
const U& ModulesHolder<Ts...>::getModule() const
{
	return std::get<U>(_modules);
}

template <typename... Ts>
template <typename F, size_t... Ns>
void ModulesHolder<Ts...>::_forEach(F&& function, std::index_sequence<Ns...>)
{
	(std::invoke(function, std::get<Ns>(_modules)), ...);
}

template <typename... Ts>
template <typename F, size_t... Ns>
void ModulesHolder<Ts...>::_forEach(F&& function, std::index_sequence<Ns...>) const
{
	(std::invoke(function, std::get<Ns>(_modules)), ...);
}

template <typename... Ts>
template <typename U, typename F, size_t... Ns>
void ModulesHolder<Ts...>::_forEach(F&& function, std::index_sequence<Ns...>)
{
	(_forEachHelper<Ns, U>(std::forward<F>(function)), ...);
}

template <typename... Ts>
template <typename U, typename F, size_t... Ns>
void ModulesHolder<Ts...>::_forEach(F&& function, std::index_sequence<Ns...>) const
{
	(_forEachHelper<Ns, U>(std::forward<F>(function)), ...);
}

template <typename... Ts>
template <size_t N, typename U, typename F>
void ModulesHolder<Ts...>::_forEachHelper(F&& function)
{
	if constexpr(std::is_base_of_v<U, NthModule_t<N>>)
		std::invoke(function, std::get<N>(_modules));
}

template <typename... Ts>
template <size_t N, typename U, typename F>
void ModulesHolder<Ts...>::_forEachHelper(F&& function) const
{
	if constexpr(std::is_base_of_v<U, NthModule_t<N>>)
		std::invoke(function, std::get<N>(_modules));
}

}
