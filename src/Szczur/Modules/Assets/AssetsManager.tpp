namespace rat
{

template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::load(const std::string& _Path)
{
	return _getContainer<TType>().try_emplace(_getKeyFromPath(_Path)).first->second.load(_Path);
}

template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::unload(const std::string& _Path)
{
	if (auto It = _find<TType>(_getKeyFromPath(_Path)); It != _getContainer<TType>().end()) {
		if (It->second.unload()) {
			_getContainer<TType>().erase(It);
			return true;
		}
	}

	return false;
}

template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::isLoaded(const std::string& _Path) const
{
	return _find<TType>(_getKeyFromPath(_Path)) != _getContainer<TType>().end();
}

template<typename... TTypes>
template<typename TType>
TType* AssetsManager<TTypes...>::getPtr(const std::string& path)
{
	if (auto It = _find(_getKeyFromPath(path)); It != _getContainer<TType>().end()) {
		return nullptr;// todo
	}

	return nullptr;
}

template<typename... TTypes>
template<typename TType>
const TType* AssetsManager<TTypes...>::getPtr(const std::string& path) const
{
	if (auto It = _find(_getKeyFromPath(path)); It != _getContainer<TType>().end()) {
		return nullptr;// todo
	}

	return nullptr;
}

template<typename... TTypes>
template<typename TType>
TType& AssetsManager<TTypes...>::get(const std::string& path)
{
	return *getPtr<TType>(path);
}

template<typename... TTypes>
template<typename TType>
const TType& AssetsManager<TTypes...>::get(const std::string& path) const
{
	return *getPtr<TType>(path);
}

template<typename... TTypes>
typename AssetsManager<TTypes...>::Key_t AssetsManager<TTypes...>::_getKeyFromPath(const std::string& _Path) const
{
	return fnv1a_64(_Path.begin(), _Path.end());
}

template<typename... TTypes>
template<typename TType>
typename AssetsManager<TTypes...>::template Container_t<TType>& AssetsManager<TTypes...>::_getContainer()
{
	return std::get<Container_t<TType>>(_holder);
}

template<typename... TTypes>
template<typename TType>
const typename AssetsManager<TTypes...>::template Container_t<TType>& AssetsManager<TTypes...>::_getContainer() const
{
	return std::get<Container_t<TType>>(_holder);
}

template<typename... TTypes>
template<typename TType>
typename AssetsManager<TTypes...>::template Iterator_t<TType> AssetsManager<TTypes...>::_find(const Key_t& _Key)
{
	return _getContainer<TType>().find(_Key);
}

template<typename... TTypes>
template<typename TType>
typename AssetsManager<TTypes...>::template ConstIterator_t<TType> AssetsManager<TTypes...>::_find(const Key_t& _Key) const
{
	return _getContainer<TType>().find(_Key);
}

}
