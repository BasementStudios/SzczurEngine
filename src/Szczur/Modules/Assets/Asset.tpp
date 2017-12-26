namespace rat
{

template<typename TType>
template<typename... TArgs>
Asset<TType>::Asset(TArgs&&... args) :
	_ptr(Traits_t::create(std::forward<TArgs>(args)...)), _refCount(0)
{

}

template<typename TType>
Asset<TType>::~Asset()
{
	delete _ptr;
}

template<typename TType>
template<typename... TArgs>
bool Asset<TType>::load(TArgs&&... args)
{
	++_refCount;
	return _refCount > 1 ? true : Traits_t::load(*_ptr, std::forward<TArgs>(args)...);
}

template<typename TType>
template<typename... TArgs>
bool Asset<TType>::unload(TArgs&&... args)
{
	if (_refCount == 0)
		return false;

	if (--_refCount == 0) {
		Traits_t::unload(*_ptr, std::forward<TArgs>(args)...);
		return true;
	}

	return false;
}

template<typename TType>
template<typename... TArgs>
bool Asset<TType>::forceUnload(TArgs&&... args)
{
	_refCount = 0;
	Traits_t::unload(*_ptr, std::forward<TArgs>(args)...);
	return true;
}

template<typename TType>
typename Asset<TType>::Pointer_t Asset<TType>::getPtr()
{
	return _refCount > 0 ? _ptr : nullptr;
}

template<typename TType>
typename Asset<TType>::ConstPointer_t Asset<TType>::getPtr() const
{
	return _refCount > 0 ? _ptr : nullptr;
}

template<typename TType>
typename Asset<TType>::Reference_t Asset<TType>::get()
{
	return *getPtr();
}

template<typename TType>
typename Asset<TType>::ConstReference_t Asset<TType>::get() const
{
	return *getPtr();
}

template<typename TType>
bool Asset<TType>::isLoaded() const
{
	return _refCount > 0;
}

}
