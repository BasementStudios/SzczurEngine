#pragma once

/** @file Asset.hpp
 ** @description Header file with Asset class. 
 ** @author Patryk (Stritch)
 ** @auhtor Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <type_traits> 					// add_lvalue_reference

#include "AssetTraits.hpp"

namespace rat
{

/** @class Asset
 ** @desciption 
 **/
template<typename TType>
class Asset
{
	/* Types */
public:
	using Value_t  = TType;
	using Traits_t = AssetTraits<Value_t>;



	/* Variables */
private:
	TType* _ptr;
public:
	std::size_t referenceCount;

	

	/* Operators */
public:
	template<typename... Us>
	Asset(Us&&... args);

	// Disable copy constructors and operators
	Asset(const Asset&) = delete;
	Asset& operator = (const Asset&) = delete;

	Asset(Asset&&) = default;
	Asset& operator = (Asset&&) = default;

	~Asset() { if (_ptr) delete _ptr; }

	typename std::add_lvalue_reference<Value_t>::type operator*() const { return *_ptr; }



	/* Methods */
public:
	template<typename... Us>
	TType* load(Us&&... args);

	template<typename... Us>
	bool unload(Us&&... args);

	template<typename... Us>
	bool forceUnload(Us&&... args);

	Value_t*       get()		{ return _ptr; }
	const Value_t* get() const	{ return _ptr; }

	Value_t*       ptr()		{ return  _ptr; }
	const Value_t* ptr() const	{ return  _ptr; }
	Value_t&       ref()		{ return *_ptr; }
	const Value_t& ref() const	{ return *_ptr; }

	bool isLoaded() const
		{ return referenceCount > 0; }

};

}

#include "Asset.tpp"
