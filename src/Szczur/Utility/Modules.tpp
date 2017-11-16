namespace rat {
	template<typename... TModules>
	template<typename UTuple>
	ModuleBase<TModules...>::ModuleBase(UTuple&& modules) :
		_modules(std::get<TModules>(modules)...) {

	}

	template<typename... TModules>
	template<typename TModule>
	TModule& ModuleBase<TModules...>::getModule() {
		return std::get<TModule&>(_modules);
	}

	template<typename... TModules>
	template<typename TModule>
	const TModule& ModuleBase<TModules...>::getModule() const {
		return std::get<TModule&>(_modules);
	}

	template<typename... TModules>
	ModulesHolder<TModules...>::ModulesHolder() :
		_modules(((void)Dummy<TModules>{}, _modules)...) {

	}

	template<typename... TModules>
	template<typename TModule>
	TModule& ModulesHolder<TModules...>::getModule() {
		return std::get<TModule>(_modules);
	}

	template<typename... TModules>
	template<typename TModule>
	const TModule& ModulesHolder<TModules...>::getModule() const {
		return std::get<TModule>(_modules);
	}
}
