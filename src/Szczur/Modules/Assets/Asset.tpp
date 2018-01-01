/** @file Asset.tpp
 ** @description Templates implemtation file for Asset class.
 ** @module Assets
 ** @author Patryk (Stritch)
 ** @auhtor Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <utility>						// forward

namespace rat
{

template<typename TType>
template<typename... TArgs>
Asset<TType>::Asset(TArgs&&... args) :
	_ptr(Traits_t::create(std::forward<TArgs>(args)...)), referenceCount(0)
{}

template<typename TType>
template<typename... TArgs>
TType* Asset<TType>::load(TArgs&&... args)
{
	if (++referenceCount > 1) {
		return _ptr;
	}
	return Traits_t::load(*_ptr, std::forward<TArgs>(args)...);
}

template<typename TType>
template<typename... TArgs>
void Asset<TType>::unload(TArgs&&... args)
{
	if (--referenceCount == 1) {
		Traits_t::unload(*_ptr, std::forward<TArgs>(args)...);
	}
}

template<typename TType>
template<typename... TArgs>
void Asset<TType>::forceUnload(TArgs&&... args)
{
	referenceCount = 0;
	Traits_t::unload(*_ptr, std::forward<TArgs>(args)...);
}

}
