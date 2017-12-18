namespace rat
{

template<typename T>
template<typename... Us>
Asset<T>::Asset(Us&&... args) :
	_holder(AssetTraits<Value_t>::create(std::forward<Us>(args)...)), _refCount(0)
{

}

template<typename T>
template<typename... Us>
bool Asset<T>::load(Us&&... args)
{
	++_refCount;

	return _refCount > 1 ? true : AssetTraits<Value_t>::load(*_holder, std::forward<Us>(args)...);
}

template<typename T>
bool Asset<T>::unload()
{
	if (_refCount == 0)
		return false;

	if (--_refCount == 0) {
		AssetTraits<Value_t>::unload(*_holder);
		_holder.reset();
		return true;
	}

	return false;
}

template<typename T>
typename Asset<T>::Value_t* Asset<T>::getPtr()
{
	return _refCount != 0 ? _holder.get() : nullptr;
}

template<typename T>
const typename Asset<T>::Value_t* Asset<T>::getPtr() const
{
	return _refCount != 0 ? _holder.get() : nullptr;
}

template<typename T>
typename Asset<T>::Value_t& Asset<T>::get()
{
	return *getPtr();
}

template<typename T>
const typename Asset<T>::Value_t& Asset<T>::get() const
{
	return *getPtr();
}

}
