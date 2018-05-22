namespace rat
{

template <typename... Ts>
Module<Ts...>::Module()
	: _modulesRefs { *detail::globalPtr<Ts>... }
{

}

template <typename... Ts>
template <typename U>
U& Module<Ts...>::getModule()
{
	static_assert((std::is_same_v<U, Ts> || ...), "Demanded module is not in dependencies, check 'public Module<Deps...>'");

	return std::get<U&>(_modulesRefs);
}

template <typename... Ts>
template <typename U>
const U& Module<Ts...>::getModule() const
{
	static_assert((std::is_same_v<U, Ts> || ...), "Demanded module is not in dependencies, check 'public Module<Deps...>'");

	return std::get<U&>(_modulesRefs);
}

}