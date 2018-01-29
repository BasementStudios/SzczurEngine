namespace rat
{

template <typename... Ts>
template <typename U>
U& AssetsManager<Ts...>::load(const std::string& path)
{
	if (auto it = _getContainer<U>().try_emplace(_obtainKey(path)).first; it->second.load(path)) {
		return it->second.getRef();
	}
	else {
		LOG_WARN("Cannot load ", AssetTraits<U>::getName(), " from path ", std::quoted(path), ", fallback reference returned");
		_getContainer<U>().erase(it);
	}

	return std::get<0>(std::get<Held_t<U>>(_holder)).getRef();
}

template <typename... Ts>
template <typename U>
bool AssetsManager<Ts...>::unload(const std::string& path)
{
	if (auto it = _find<U>(_obtainKey(path)); it != _getContainer<U>().end()) {
		if (it->second.unload()) {
			_getContainer<U>().erase(it);
			return true;
		}
	}

	return false;
}

template <typename... Ts>
template <typename U>
bool AssetsManager<Ts...>::isLoaded(const std::string& path) const
{
	return _find<U>(_obtainKey(path)) != _getContainer<U>().end();
}

template <typename... Ts>
template <typename U>
U* AssetsManager<Ts...>::getPtr(const std::string& path)
{
	if (auto it = _find<U>(_obtainKey(path)); it != _getContainer<U>().end()) {
		return it->second.getPtr();
	}

	LOG_WARN("Cannot find ", AssetTraits<U>::getName(), " from path ", std::quoted(path), ", fallback pointer returned");
	return std::get<0>(std::get<Held_t<U>>(_holder)).getPtr();
}

template <typename... Ts>
template <typename U>
const U* AssetsManager<Ts...>::getPtr(const std::string& path) const
{
	if (auto it = _find<U>(_obtainKey(path)); it != _getContainer<U>().end()) {
		return it->second.getPtr();
	}

	LOG_WARN("Cannot find ", AssetTraits<U>::getName(), " from path ", std::quoted(path), ", fallback pointer returned");
	return std::get<0>(std::get<Held_t<U>>(_holder)).getPtr();
}

template <typename... Ts>
template <typename U>
U& AssetsManager<Ts...>::getRef(const std::string& path)
{
	if (auto it = _find<U>(_obtainKey(path)); it != _getContainer<U>().end()) {
		return it->second.getRef();
	}

	LOG_WARN("Cannot find ", AssetTraits<U>::getName(), " from path ", std::quoted(path), ", fallback reference returned");
	return std::get<0>(std::get<Held_t<U>>(_holder)).getRef();
}

template <typename... Ts>
template <typename U>
const U& AssetsManager<Ts...>::getRef(const std::string& path) const
{
	if (auto it = _find<U>(_obtainKey(path)); it != _getContainer<U>().end()) {
		return it->second.getRef();
	}

	LOG_WARN("Cannot find ", AssetTraits<U>::getName(), " from path ", std::quoted(path), ", fallback reference returned");
	return std::get<0>(std::get<Held_t<U>>(_holder)).getRef();
}

template <typename... Ts>
typename AssetsManager<Ts...>::Key_t AssetsManager<Ts...>::_obtainKey(const std::string& path) const
{
	return fnv1a_64(path.begin(), path.end());
}

template <typename... Ts>
template <typename U>
typename AssetsManager<Ts...>::DEPENDENT_TEMPLATE_SCOPE Container_t<U>& AssetsManager<Ts...>::_getContainer()
{
	return std::get<1>(std::get<Held_t<U>>(_holder));
}

template <typename... Ts>
template <typename U>
const typename AssetsManager<Ts...>::DEPENDENT_TEMPLATE_SCOPE Container_t<U>& AssetsManager<Ts...>::_getContainer() const
{
	return std::get<1>(std::get<Held_t<U>>(_holder));
}

template <typename... Ts>
template <typename U>
typename AssetsManager<Ts...>::DEPENDENT_TEMPLATE_SCOPE Iterator_t<U> AssetsManager<Ts...>::_find(const Key_t& _Key)
{
	return _getContainer<U>().find(_Key);
}

template <typename... Ts>
template <typename U>
typename AssetsManager<Ts...>::DEPENDENT_TEMPLATE_SCOPE ConstIterator_t<U> AssetsManager<Ts...>::_find(const Key_t& _Key) const
{
	return _getContainer<U>().find(_Key);
}

}
