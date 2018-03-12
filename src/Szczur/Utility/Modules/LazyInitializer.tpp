namespace rat
{

template <typename T>
LazyInitializer<T>::LazyInitializer(const LazyInitializer& rhs)
{
	init(rhs);
}

template <typename T>
LazyInitializer& LazyInitializer<T>::operator = (const LazyInitializer& rhs)
{
	getRef() = rhs.getRef();
}

template <typename T>
LazyInitializer<T>::LazyInitializer(LazyInitializer&& rhs)
{
	init(std::move(rhs));
}

template <typename T>
LazyInitializer& LazyInitializer<T>::operator = (LazyInitializer&& rhs)
{
	getRef() = std::move(rhs.getRef());
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
Pointer_t LazyInitializer<T>::getPtr()
{
	return reinterpret_cast<Pointer_t>(_getBufferPtr());
}

template <typename T>
ConstPointer_t LazyInitializer<T>::getPtr() const
{
	return reinterpret_cast<ConstPointer_t>(_getBufferPtr());
}

template <typename T>
Pointer_t LazyInitializer<T>::operator -> ()
{
	return getPtr();
}

template <typename T>
ConstPointer_t LazyInitializer<T>::operator -> () const
{
	return getPtr();
}

template <typename T>
Reference_t LazyInitializer<T>::getRef()
{
	return *getPtr();
}

template <typename T>
ConstReference_t LazyInitializer<T>::getRef() const
{
	return *getPtr();
}

template <typename T>
Reference_t LazyInitializer<T>::operator * ()
{
	return getRef();
}

template <typename T>
ConstReference_t LazyInitializer<T>::operator * () const
{
	return getRef();
}

template <typename T>
void* LazyInitializer<T>::_getBufferPtr()
{
	return &_buffer;
}

template <typename T>
const void* LazyInitializer<T>::_getBufferPtr() const
{
	return &_buffer;
}

}
