template<typename... Ts>
ModulesHolder<Ts...>::ModulesHolder() :
	_modules(((void)Dummy<Ts>{}, _modules)...) {

}

template<typename... Ts>
template<typename T>
T& ModulesHolder<Ts...>::get() {
	return std::get<T>(_modules);
}

template<typename... Ts>
template<typename T>
const T& ModulesHolder<Ts...>::get() const {
	return std::get<T>(_modules);
}
