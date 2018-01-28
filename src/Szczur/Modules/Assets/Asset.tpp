namespace rat
{

template <typename T>
Asset<T>::Asset() :
	_ptr(Traits_t::create()), _refCount(0)
{
	LOG_INFO(this, " : Created asset of type ", AssetTraits<T>::getName());
}

template <typename T>
Asset<T>::Asset(Asset&& rhs) noexcept :
	_ptr(rhs._ptr), _refCount(rhs._refCount)
{
	rhs._ptr = nullptr;
	rhs._refCount = 0;
}

template <typename T>
Asset<T>& Asset<T>::operator = (Asset&& rhs) noexcept
{
	if (this != &rhs) {
		_clear();
		_ptr = rhs._ptr;
		_refCount = rhs._refCount;
		rhs._ptr = nullptr;
		rhs._refCount = 0;
	}

	return *this;
}

template <typename T>
Asset<T>::~Asset()
{
	_clear();
}

template <typename T>
template <typename... Us>
bool Asset<T>::load(Us&&... args)
{
	return ++_refCount > 1 ? true : Traits_t::load(getRef(), std::forward<Us>(args)...);
}

template <typename T>
bool Asset<T>::unload()
{
	if (_refCount == 0)
		return false;

	if (--_refCount == 0) {
		Traits_t::unload(getRef());
		return true;
	}

	return false;
}

template <typename T>
bool Asset<T>::forceUnload()
{
	_refCount = 0;
	Traits_t::unload(getRef());
	return true;
}

template <typename T>
typename Asset<T>::Pointer_t Asset<T>::getPtr()
{
	return _ptr;
}

template <typename T>
typename Asset<T>::ConstPointer_t Asset<T>::getPtr() const
{
	return _ptr;
}

template <typename T>
typename Asset<T>::Reference_t Asset<T>::getRef()
{
	LOG_ERROR_IF(getPtr() == nullptr, this, " : Trying to dereference nullptr in asset of type ", AssetTraits<Value_t>::getName());
	return *getPtr();
}

template <typename T>
typename Asset<T>::ConstReference_t Asset<T>::getRef() const
{
	LOG_ERROR_IF(getPtr() == nullptr, this, " : Trying to dereference nullptr in asset of type ", AssetTraits<Value_t>::getName());
	return *getPtr();
}

template <typename T>
bool Asset<T>::isValid() const
{
	return _ptr != nullptr;
}

template <typename T>
bool Asset<T>::isLoaded() const
{
	return _refCount > 0;
}

template <typename T>
void Asset<T>::_clear()
{
	if (isValid()) {
		if (isLoaded()) {
			Traits_t::unload(getRef());
		}
		delete _ptr;
		LOG_INFO(this, " : Destroyed asset of type ", AssetTraits<Value_t>::getName());
	}
}

}
