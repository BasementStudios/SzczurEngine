namespace rat
{

template <typename... Ts>
Module<Ts...>::Module() :
	_modulesRefs(*modulePtr_v<Ts>...)
{

}

template <typename... Ts>
template <typename U>
U& Module<Ts...>::getModule()
{
	static_assert(_dependsOn<U>());

	return std::get<U&>(_modulesRefs);
}

template <typename... Ts>
template <typename U>
const U& Module<Ts...>::getModule() const
{
	static_assert(_dependsOn<U>());

	return std::get<U&>(_modulesRefs);
}

template <typename... Ts>
template <typename U>
constexpr bool Module<Ts...>::_dependsOn()
{
	return (std::is_same_v<U, Ts> || ...);
}

}