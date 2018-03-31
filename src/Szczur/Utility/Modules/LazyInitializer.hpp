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

	LazyInitializer(const LazyInitializer& rhs);

	LazyInitializer& operator = (const LazyInitializer& rhs);

	LazyInitializer(LazyInitializer&& rhs);

	LazyInitializer& operator = (LazyInitializer&& rhs);

	~LazyInitializer();

	template <typename... Us>
	void init(Us&&... args);

	Pointer_t getPtr();
	ConstPointer_t getPtr() const;

	Pointer_t operator -> ();
	ConstPointer_t operator -> () const;

	Reference_t getRef();
	ConstReference_t getRef() const;

	Reference_t operator * ();
	ConstReference_t operator * () const;

private:

	void* _getBufferPtr();
	const void* _getBufferPtr() const;

	Buffer_t _buffer;

};

}

#include "LazyInitializer.tpp"
