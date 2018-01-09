namespace rat
{

template <typename U>
U& Assets::load(const std::string& path)
{
	return _manager.load<U>(path);
}

template <typename U>
bool Assets::unload(const std::string& path)
{
	return _manager.unload<U>(path);
}

template <typename U>
bool Assets::isLoaded(const std::string& path) const
{
	return _manager.isLoaded<U>(path);
}

template <typename U>
U* Assets::getPtr(const std::string& path)
{
	return _manager.getPtr<U>(path);
}

template <typename U>
const U* Assets::getPtr(const std::string& path) const
{
	return _manager.getPtr<U>(path);
}

template <typename U>
U& Assets::getRef(const std::string& path)
{
	return _manager.getRef<U>(path);
}

template <typename U>
const U& Assets::getRef(const std::string& path) const
{
	return _manager.getRef<U>(path);
}

}
