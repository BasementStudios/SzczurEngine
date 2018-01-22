namespace rat
{

template <typename... Ts>
constexpr size_t ModulesHolder<Ts...>::modulesCount()
{
	return std::tuple_size_v<std::decay_t<Holder_t>>;
}

template <typename... Ts>
template <typename U, typename... Us>
void ModulesHolder<Ts...>::initModule(Us&&... args)
{
	std::get<Held_t<U>>(_modules).init(_modules, std::forward<Us>(args)...);
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
