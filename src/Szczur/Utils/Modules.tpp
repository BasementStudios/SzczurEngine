template<typename... Ts>
template<typename... Us>
ModuleBase<Ts...>::ModuleBase(std::tuple<Us...>& modules) :
	_modules(std::get<Ts>(modules)...) {

}

template<typename... Ts>
template<typename T>
T& ModuleBase<Ts...>::getModule() {
	return std::get<T&>(_modules);
}

template<typename... Ts>
template<typename T>
const T& ModuleBase<Ts...>::getModule() const {
	return std::get<T&>(_modules);
}

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
