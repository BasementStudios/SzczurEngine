namespace rat
{

template<typename... Ts>
template<typename U>
bool AssetsManager<Ts...>::load(const std::string& _Path)
{
	return _getContainer<U>().try_emplace(_getKeyFromPath(_Path)).first->second.load(_Path);
}

template<typename... Ts>
template<typename U>
bool AssetsManager<Ts...>::unload(const std::string& _Path)
{
	if(auto It = _find<U>(_getKeyFromPath(_Path)); It != _getContainer<U>().end()) {
		if(It->second.unload()) {
			_getContainer<U>().erase(It);
			return true;
		}
	}

	return false;
}

template<typename... Ts>
template<typename U>
bool AssetsManager<Ts...>::isLoaded(const std::string& _Path) const
{
	return _find<U>(_getKeyFromPath(_Path)) != _getContainer<U>().end();
}

template<typename... Ts>
template<typename U>
U* AssetsManager<Ts...>::getPtr(const std::string& path)
{
	if(auto It = _find(_getKeyFromPath(path)); It != _getContainer<U>().end()) {
		return nullptr;// todo
	}

	return nullptr;
}

template<typename... Ts>
template<typename U>
const U* AssetsManager<Ts...>::getPtr(const std::string& path) const
{
	if(auto It = _find(_getKeyFromPath(path)); It != _getContainer<U>().end()) {
		return nullptr;// todo
	}

	return nullptr;
}

template<typename... Ts>
template<typename U>
U& AssetsManager<Ts...>::get(const std::string& path)
{
	return *getPtr<U>();
}

template<typename... Ts>
template<typename U>
const U& AssetsManager<Ts...>::get(const std::string& path) const
{
	return *getPtr<U>();
}

template<typename... Ts>
typename AssetsManager<Ts...>::Key_t AssetsManager<Ts...>::_getKeyFromPath(const std::string& _Path) const
{
	return fnv1a_64(_Path.begin(), _Path.end());
}

template<typename... Ts>
template<typename U>
typename AssetsManager<Ts...>::template Container_t<U>& AssetsManager<Ts...>::_getContainer()
{
	return std::get<Container_t<U>>(_holder);
}

template<typename... Ts>
template<typename U>
const typename AssetsManager<Ts...>::template Container_t<U>& AssetsManager<Ts...>::_getContainer() const
{
	return std::get<Container_t<U>>(_holder);
}

template<typename... Ts>
template<typename U>
typename AssetsManager<Ts...>::template Iterator_t<U> AssetsManager<Ts...>::_find(const Key_t& _Key)
{
	return _getContainer<U>().find(_Key);
}

template<typename... Ts>
template<typename U>
typename AssetsManager<Ts...>::template ConstIterator_t<U> AssetsManager<Ts...>::_find(const Key_t& _Key) const
{
	return _getContainer<U>().find(_Key);
}

}
