namespace rat
{

template <typename T>
FallbackAsset<T>::FallbackAsset() :
	_ptr(Traits_t::createFallback())
{
	LOG_INFO(this, " : Created fallback asset of type ", AssetTraits<Value_t>::getName());
}

template <typename T>
FallbackAsset<T>::FallbackAsset(FallbackAsset&& rhs) noexcept :
	_ptr(rhs._ptr)
{
	rhs._ptr = nullptr;
}

template <typename T>
FallbackAsset<T>& FallbackAsset<T>::operator = (FallbackAsset&& rhs) noexcept
{
	if (this != &rhs) {
		_clear();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
	}

	return *this;
}

template <typename T>
FallbackAsset<T>::~FallbackAsset()
{
	_clear();
}

template <typename T>
typename FallbackAsset<T>::Pointer_t FallbackAsset<T>::getPtr()
{
	return _ptr;
}

template <typename T>
typename FallbackAsset<T>::ConstPointer_t FallbackAsset<T>::getPtr() const
{
	return _ptr;
}

template <typename T>
typename FallbackAsset<T>::Reference_t FallbackAsset<T>::getRef()
{
	LOG_ERROR_IF(getPtr() == nullptr, this, " : Trying to dereference nullptr in fallback asset of type ", AssetTraits<Value_t>::getName());
	return *getPtr();
}

template <typename T>
typename FallbackAsset<T>::ConstReference_t FallbackAsset<T>::getRef() const
{
	LOG_ERROR_IF(getPtr() == nullptr, this, " : Trying to dereference nullptr in fallback asset of type ", AssetTraits<Value_t>::getName());
	return *getPtr();
}

template <typename T>
bool FallbackAsset<T>::isValid() const
{
	return _ptr != nullptr;
}

template <typename T>
bool FallbackAsset<T>::isLoaded() const
{
	return isValid();
}

template <typename T>
void FallbackAsset<T>::_clear()
{
	if (isValid()) {
		Traits_t::unload(getRef());
		delete _ptr;
		LOG_INFO(this, " : Destroyed fallback asset of type ", AssetTraits<Value_t>::getName());
	}
}

}
