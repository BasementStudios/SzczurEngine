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
Module<Ts...>::Module(U&& tuple) :
	_modulesRefs(std::get<LazyInitializer<Ts>>(tuple)...)
{

}

template <typename... Ts>
template <typename U>
U& Module<Ts...>::getModule()
{
	static_assert(dependsOn<U>());

	return *std::get<Held_t<U>>(_modulesRefs);
}

template <typename... Ts>
template <typename U>
const U& Module<Ts...>::getModule() const
{
	static_assert(dependsOn<U>());

	return *std::get<Held_t<U>>(_modulesRefs);
}

template <typename... Ts>
constexpr size_t ModulesHolder<Ts...>::modulesCount()
{
	return std::tuple_size_v<std::decay_t<Holder_t>>;
}

template <typename... Ts>
template <typename U>
U& ModulesHolder<Ts...>::getModule()
{
	return *std::get<Held_t<U>>(_modules);
}

template <typename... Ts>
template <typename U>
const U& ModulesHolder<Ts...>::getModule() const
{
	return *std::get<Held_t<U>>(_modules);
}

}
