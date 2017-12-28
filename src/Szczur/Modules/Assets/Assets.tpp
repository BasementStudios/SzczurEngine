namespace rat
{

template<typename TType>
bool Assets::load(const std::string& path)
{
	return _manager.load<TType>(path);
}

template<typename TType>
bool Assets::unload(const std::string& path)
{
	return _manager.unload<TType>(path);
}

template<typename TType>
bool Assets::isLoaded(const std::string& path) const
{
	return _manager.isLoaded<TType>(path);
}

template<typename TType>
TType* Assets::getPtr(const std::string& path)
{
	return _manager.getPtr<TType>(path);
}

template<typename TType>
const TType* Assets::getPtr(const std::string& path) const
{
	return _manager.getPtr<TType>(path);
}

template<typename TType>
TType& Assets::get(const std::string& path)
{
	return _manager.get<TType>(path);
}

template<typename TType>
const TType& Assets::get(const std::string& path) const
{
	return _manager.get<TType>(path);
}

}
