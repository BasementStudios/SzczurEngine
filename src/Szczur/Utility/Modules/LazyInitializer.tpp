namespace rat
{

template <typename T>
LazyInitializer<T>::LazyInitializer(const LazyInitializer<T>& rhs)
{
	init(rhs);
}

template <typename T>
LazyInitializer<T>& LazyInitializer<T>::operator = (const LazyInitializer<T>& rhs)
{
	getRef() = rhs.getRef();

	return *this;
}

template <typename T>
LazyInitializer<T>::LazyInitializer(LazyInitializer<T>&& rhs)
{
	init(std::move(rhs));
}

template <typename T>
LazyInitializer<T>& LazyInitializer<T>::operator = (LazyInitializer<T>&& rhs)
{
	getRef() = std::move(rhs.getRef());

	return *this;
}

template <typename T>
LazyInitializer<T>::~LazyInitializer()
{
	getRef().~Held_t();
}

template <typename T>
template <typename... Us>
void LazyInitializer<T>::init(Us&&... args)
{
	new (_getBufferPtr()) Held_t(std::forward<Us>(args)...);
}

template <typename T>
typename LazyInitializer<T>::Pointer_t LazyInitializer<T>::getPtr()
{
	return reinterpret_cast<Pointer_t>(_getBufferPtr());
}

template <typename T>
typename LazyInitializer<T>::ConstPointer_t LazyInitializer<T>::getPtr() const
{
	return reinterpret_cast<ConstPointer_t>(_getBufferPtr());
}

template <typename T>
typename LazyInitializer<T>::Reference_t LazyInitializer<T>::getRef()
{
	return *getPtr();
}

template <typename T>
typename LazyInitializer<T>::ConstReference_t LazyInitializer<T>::getRef() const
{
	return *getPtr();
}

template <typename T>
void* LazyInitializer<T>::_getBufferPtr()
{
	return std::addressof(_buffer);
}

template <typename T>
const void* LazyInitializer<T>::_getBufferPtr() const
{
	return std::addressof(_buffer);
}

}
