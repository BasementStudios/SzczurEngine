namespace rat
{

template<typename T>
template<typename... Us>
Asset<T>::Asset(Us&&... args) :
	_ptr(AssetTraits<Value_t>::create(std::forward<Us>(args)...)), _refCount(0)
{

}

template<typename T>
Asset<T>::~Asset()
{
	delete _ptr;
}

template<typename T>
template<typename... Us>
bool Asset<T>::load(Us&&... args)
{
	++_refCount;

	return _refCount > 1 ? true : AssetTraits<Value_t>::load(*_ptr, std::forward<Us>(args)...);
}

template<typename T>
template<typename... Us>
bool Asset<T>::unload(Us&&... args)
{
	if (_refCount == 0)
		return false;

	if (--_refCount == 0) {
		AssetTraits<Value_t>::unload(*_ptr, std::forward<Us>(args)...);
		return true;
	}

	return false;
}

template<typename T>
typename Asset<T>::Pointer_t Asset<T>::getPtr()
{
	return _refCount != 0 ? _ptr : nullptr;
}

template<typename T>
typename Asset<T>::ConstPointer_t Asset<T>::getPtr() const
{
	return _refCount != 0 ? _ptr : nullptr;
}

template<typename T>
typename Asset<T>::Reference_t Asset<T>::get()
{
	return *getPtr();
}

template<typename T>
typename Asset<T>::ConstReference_t Asset<T>::get() const
{
	return *getPtr();
}

}
