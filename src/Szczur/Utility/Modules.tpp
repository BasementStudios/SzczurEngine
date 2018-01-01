namespace rat
{

template<typename... TModules>
template<typename TModule>
constexpr bool Module<TModules...>::dependsOn()
{
	return (std::is_same_v<TModule, TModules> || ...);
}

template<typename... TModules>
constexpr size_t Module<TModules...>::dependenciesCount()
{
	return sizeof...(TModules);
}

template<typename... TModules>
template<typename TModule>
Module<TModules...>::Module(TModule&& modules) :
	_modulesRefs(std::get<TModules>(modules)...)
{

}

template<typename... TModules>
template<typename TModule>
TModule& Module<TModules...>::_getModule()
{
	static_assert(dependsOn<TModule>());

	return std::get<TModule&>(_modulesRefs);
}

template<typename... TModules>
template<typename TModule>
const TModule& Module<TModules...>::_getModule() const
{
	static_assert(dependsOn<TModule>());

	return std::get<TModule&>(_modulesRefs);
}

template<typename... TModules>
constexpr size_t ModulesHolder<TModules...>::modulesCount()
{
	return std::tuple_size_v<std::decay_t<Holder_t>>;
}

template<typename... TModules>
ModulesHolder<TModules...>::ModulesHolder() :
	_modules(TModules{ _modules }...)
{

}

template<typename... TModules>
template<typename TFunction>
void ModulesHolder<TModules...>::forEach(TFunction&& function)
{
	_forEach(std::forward<TFunction>(function), std::make_index_sequence<modulesCount()>{});
}

template<typename... TModules>
template<typename TFunction>
void ModulesHolder<TModules...>::forEach(TFunction&& function) const
{
	_forEach(std::forward<TFunction>(function), std::make_index_sequence<modulesCount()>{});
}

template<typename... TModules>
template<typename TModule, typename TFunction>
void ModulesHolder<TModules...>::forEach(TFunction&& function)
{
	_forEach<TModule>(std::forward<TFunction>(function), std::make_index_sequence<modulesCount()>{});
}

template<typename... TModules>
template<typename TModule, typename TFunction>
void ModulesHolder<TModules...>::forEach(TFunction&& function) const
{
	_forEach<TModule>(std::forward<TFunction>(function), std::make_index_sequence<modulesCount()>{});
}

template<typename... TModules>
template<typename TModule>
TModule& ModulesHolder<TModules...>::getModule()
{
	return std::get<TModule>(_modules);
}

template<typename... TModules>
template<typename TModule>
const TModule& ModulesHolder<TModules...>::getModule() const
{
	return std::get<TModule>(_modules);
}

template<typename... TModules>
template<typename TFunction, size_t... TIndices>
void ModulesHolder<TModules...>::_forEach(TFunction&& function, std::index_sequence<TIndices...>)
{
	(std::invoke(function, std::get<TIndices>(_modules)), ...);
}

template<typename... TModules>
template<typename TFunction, size_t... TIndices>
void ModulesHolder<TModules...>::_forEach(TFunction&& function, std::index_sequence<TIndices...>) const
{
	(std::invoke(function, std::get<TIndices>(_modules)), ...);
}

template<typename... TModules>
template<typename TModule, typename TFunction, size_t... TIndices>
void ModulesHolder<TModules...>::_forEach(TFunction&& function, std::index_sequence<TIndices...>)
{
	(_forEachHelper<TIndices, TModule>(std::forward<TFunction>(function)), ...);
}

template<typename... TModules>
template<typename TModule, typename TFunction, size_t... TIndices>
void ModulesHolder<TModules...>::_forEach(TFunction&& function, std::index_sequence<TIndices...>) const
{
	(_forEachHelper<TIndices, TModule>(std::forward<TFunction>(function)), ...);
}

template<typename... TModules>
template<size_t TIndex, typename TModule, typename TFunction>
void ModulesHolder<TModules...>::_forEachHelper(TFunction&& function)
{
	if constexpr(std::is_base_of_v<TModule, NthModule_t<TIndex>>) std::invoke(function, std::get<TIndex>(_modules));
}

template<typename... TModules>
template<size_t TIndex, typename TModule, typename TFunction>
void ModulesHolder<TModules...>::_forEachHelper(TFunction&& function) const
{
	if constexpr(std::is_base_of_v<TModule, NthModule_t<TIndex>>) std::invoke(function, std::get<TIndex>(_modules));
}

}
