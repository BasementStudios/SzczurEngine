#pragma once

#include <type_traits>

namespace rat
{

template <typename T>
class LazyInitializer
{
public:

	using Held_t           = T;
	using Pointer_t        = Held_t*;
	using ConstPointer_t   = const Held_t*;
	using Reference_t      = Held_t&;
	using ConstReference_t = const Held_t&;
	using Buffer_t         = std::aligned_storage_t<sizeof(Held_t), alignof(Held_t)>;

	LazyInitializer() = default;

	LazyInitializer(const LazyInitializer& rhs)
	{
		init(rhs);
	}

	LazyInitializer& operator = (const LazyInitializer& rhs)
	{
		getRef() = rhs.getRef();
	}

	LazyInitializer(LazyInitializer&& rhs)
	{
		init(std::move(rhs));
	}

	LazyInitializer& operator = (LazyInitializer&& rhs)
	{
		getRef() = std::move(rhs.getRef());
	}

	~LazyInitializer()
	{
		reinterpret_cast<Pointer_t>(_getBufferPtr())->~Held_t();
	}

	template <typename... Us>
	void init(Us&&... args)
	{
		new (_getBufferPtr()) Held_t(std::forward<Us>(args)...);
	}

	Pointer_t getPtr()
	{
		return reinterpret_cast<Pointer_t>(_getBufferPtr());
	}

	ConstPointer_t getPtr() const
	{
		return reinterpret_cast<ConstPointer_t>(_getBufferPtr());
	}

	Pointer_t operator -> ()
	{
		return getPtr();
	}

	ConstPointer_t operator -> () const
	{
		return getPtr();
	}

	Reference_t getRef()
	{
		return *reinterpret_cast<Pointer_t>(_getBufferPtr());
	}

	ConstReference_t getRef() const
	{
		return *reinterpret_cast<ConstPointer_t>(_getBufferPtr());
	}

	Reference_t operator * ()
	{
		return getRef();
	}

	ConstReference_t operator * () const
	{
		return getRef();
	}

private:

	Buffer_t* _getBufferPtr()
	{
		return &_buffer;
	}

	const Buffer_t* _getBufferPtr() const
	{
		return &_buffer;
	}

	Buffer_t _buffer;

};

}
