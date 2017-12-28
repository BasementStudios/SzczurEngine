namespace rat
{

template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::load(const std::string& path)
{
	return _getContainer<TType>().try_emplace(_getKeyFromPath(path)).first->second.load(path);
}

template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::unload(const std::string& path)
{
	if (auto it = _find<TType>(_getKeyFromPath(path)); it != _getContainer<TType>().end()) {
		if (it->second.unload()) {
			_getContainer<TType>().erase(it);
			return true;
		}
	}

	return false;
}

template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::isLoaded(const std::string& path) const
{
	return _find<TType>(_getKeyFromPath(path)) != _getContainer<TType>().end();
}

template<typename... TTypes>
template<typename TType>
TType* AssetsManager<TTypes...>::getPtr(const std::string& path)
{
	if (auto it = _find(_getKeyFromPath(path)); it != _getContainer<TType>().end()) {
		return it.second.getPtr();
	}

	return nullptr;
}

template<typename... TTypes>
template<typename TType>
const TType* AssetsManager<TTypes...>::getPtr(const std::string& path) const
{
	if (auto it = _find(_getKeyFromPath(path)); it != _getContainer<TType>().end()) {
		return it.second.getPtr();
	}

	return nullptr;
}

template<typename... TTypes>
template<typename TType>
TType& AssetsManager<TTypes...>::get(const std::string& path)
{
	if (auto it = _find(_getKeyFromPath(path)); it != _getContainer<TType>().end()) {
		return it.second.get();
	}

	return nullptr;
}

template<typename... TTypes>
template<typename TType>
const TType& AssetsManager<TTypes...>::get(const std::string& path) const
{
	if (auto it = _find(_getKeyFromPath(path)); it != _getContainer<TType>().end()) {
		return it.second.get();
	}

	return nullptr;
}

template<typename... TTypes>
typename AssetsManager<TTypes...>::Key_t AssetsManager<TTypes...>::_getKeyFromPath(const std::string& path) const
{
	return fnv1a_64(path.begin(), path.end());
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
