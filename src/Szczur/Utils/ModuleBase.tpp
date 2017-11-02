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
