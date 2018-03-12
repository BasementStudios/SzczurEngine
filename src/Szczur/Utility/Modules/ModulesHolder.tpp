namespace rat
{

template <typename... Ts>
ModulesHolder<Ts...>::ModulesHolder()
{
#if defined(COMPILER_MSVC)
	using Swallow_t = int[];
	Swallow_t{ ((modulePtr_v<Ts> = std::get<Held_t<Ts>>(_modules).getPtr()), 0)... };
#else
	((modulePtr_v<Ts> = std::get<Held_t<Ts>>(_modules).getPtr()), ...);
#endif
}

template <typename... Ts>
template <typename U, typename... Us>
void ModulesHolder<Ts...>::initModule(Us&&... args)
{
	std::get<Held_t<U>>(_modules).init(std::forward<Us>(args)...);
}

template <typename... Ts>
template <typename U>
U& ModulesHolder<Ts...>::getModule()
{
	return std::get<Held_t<U>>(_modules).getRef();
}

template <typename... Ts>
template <typename U>
const U& ModulesHolder<Ts...>::getModule() const
{
	return std::get<Held_t<U>>(_modules).getRef();
}

}
