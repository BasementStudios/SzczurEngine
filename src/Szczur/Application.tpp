namespace rat
{

template <typename U, typename... Us>
inline void Application::initModule(Us&&... args)
{
	return _modules.initModule<U>(std::forward<Us>(args)...);
}

template <typename U>
inline U& Application::getModule()
{
	return _modules.getModule<U>();
}

template <typename U>
inline const U& Application::getModule() const
{
	return _modules.getModule<U>();
}

}
