namespace rat
{

template <typename... Ts>
ModulesHolder<Ts...>::ModulesHolder()
{
#if defined(COMPILER_MSVC)
	using Swallow_t = int[];
	Swallow_t{ ((detail::globalPtr<Ts> = std::addressof(getModule<Ts>())), 0)... };
#else
	((detail::globalPtr<Ts> = std::addressof(getModule<Ts>())), ...);
#endif
}

template <typename... Ts>
ModulesHolder<Ts...>::~ModulesHolder()
{
	_destroyAll(std::index_sequence_for<Ts...>{});
}

template <typename... Ts>
template <typename U, typename... Us>
void ModulesHolder<Ts...>::initModule(Us&&... args)
{
	new (getStoragePtrFor<U>()) U{ std::forward<Us>(args)... };
}

template <typename... Ts>
template <typename U>
U& ModulesHolder<Ts...>::getModule()
{
	return *reinterpret_cast<U*>(getStoragePtrFor<U>());
}

template <typename... Ts>
template <typename U>
const U& ModulesHolder<Ts...>::getModule() const
{
	return *reinterpret_cast<U*>(getStoragePtrFor<U>());
}

template <typename... Ts>
template <size_t N>
detail::NthElement_t<N, Ts...>& ModulesHolder<Ts...>::getModule()
{
	static_assert(N < sizeof...(Ts), "Desired index is not valid");

	return getModule<detail::NthElement_t<N, Ts...>>();
}

template <typename... Ts>
template <size_t N>
const detail::NthElement_t<N, Ts...>& ModulesHolder<Ts...>::getModule() const
{
	static_assert(N < sizeof...(Ts), "Desired index is not valid");

	return getModule<detail::NthElement_t<N, Ts...>>();
}

template <typename... Ts>
template <typename U>
void* ModulesHolder<Ts...>::getStoragePtrFor()
{
	return reinterpret_cast<char*>(std::addressof(_holder)) + detail::SizeOffset<0, U, Ts...>::value;
}

template <typename... Ts>
template <typename U>
const void* ModulesHolder<Ts...>::getStoragePtrFor() const
{
	return reinterpret_cast<char*>(std::addressof(_holder)) + detail::SizeOffset<0, U, Ts...>::value;
}

template <typename... Ts>
template <size_t... Ns>
void ModulesHolder<Ts...>::_destroyAll(std::index_sequence<Ns...>)
{
#if defined(COMPILER_MSVC)
	using Swallow_t = int[];
	Swallow_t{ ((_destroy<detail::NthElement_t<sizeof...(Ts) - 1u - Ns, Ts...>>()), 0)... };
#else
	((_destroy<detail::NthElement_t<sizeof...(Ts) - 1u - Ns, Ts...>>()), ...);
#endif
}

template <typename... Ts>
template <typename U>
void ModulesHolder<Ts...>::_destroy()
{
	getModule<U>().~U();
}

}
