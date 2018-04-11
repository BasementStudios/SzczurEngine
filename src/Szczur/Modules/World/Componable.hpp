#pragma once

/** @file Componable.hpp
 ** @description @todo xd
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>
#include <tuple>
#include <functional>

namespace rat
{

namespace Componable
{

// Embed all components into single class
template <typename TComponent, typename... TComponents>
struct WithComponents 
	: public virtual TComponent, public virtual WithComponents<TComponents...>
{};

template <typename TComponent>
struct WithComponents<TComponent> 
	: public virtual TComponent
{};



// Component type
template <typename TRequiredComponent, typename... TRequiredComponents>
struct Component 
	: public virtual TRequiredComponent, public virtual WithComponents<TRequiredComponents...>
{}; // @todo . get<TComponent> ref

template <typename TRequiredComponent>
struct Component<TRequiredComponent> 
	: public virtual WithComponents<TRequiredComponent>
{};



// Whole object variant type
template <typename... TComponents>
struct Object : public WithComponents<TComponents...>
{
	using ComponentsTuple_t = std::tuple<TComponents...>;
	
	Object(TComponents&&... components) 
		: TComponents(std::forward<TComponents>(components))...
	{};
};



// Traits which checks is object type has specified components
template <typename TObject, typename TComponent, typename... TComponents>
struct HasComponents
	: std::integral_constant<std::size_t, 
		std::conjunction<
			std::is_base_of<TObject, TComponent>, 
			HasComponents<TObject, TComponents...>
		>::value
	>
{};

template <typename TObject, typename TComponent>
struct HasComponents<TObject, TComponent> 
	: std::integral_constant<std::size_t, std::is_base_of<TObject, TComponent>::value>
{};



/// @todo dedu :(
/*
using ArgumentsTuple_t = std::tuple<Args...>;

using FirstArgument_t = typename std::tuple_element<0, ArgumentsTuple_t>::type;

using Object_t = std::is_same<ArgumentsTuple_t, TTypes...>{}

template <typename ArgumentsTuple_t, typename TObject, typename... TObjects>
struct ZZZZZZ
	: std::integral_constant<std::size_t, 
		(std::is_same<TObject::ComponentsTuple_t, ArgumentsTuple_t>::value)
	>
{}
*/



// Componable objects holder
template <typename TCommonComponent, typename... TTypes>
struct ObjectsHolder
{
	/* Types */
	using CommonComponent_t = TCommonComponent;
	
	template <typename TType>
	using TypeContainer_t = std::vector<TType>;
	
	using ContainersTuple_t = std::tuple<TypeContainer_t<TTypes>...>;

	/* Fields */
	ContainersTuple_t containersTuple;
	
	/* Properties */
	template <typename TType>
	inline TypeContainer_t<TType>& getContainer()
	{
		return std::get<TypeContainer_t<TType>>(containersTuple);
	}
	
	/* Methods */
	template <typename TType, typename... Args>
	inline auto& emplace(Args&&... args)
	{
		return this->getContainer<TType>().emplace_back(std::forward<Args>(args)...);
	}
	
	template <
		typename TComponent = CommonComponent_t,
		size_t Index = 0,
		size_t Size = std::tuple_size_v<ContainersTuple_t>,
		typename TCallable
	>
	inline void forEach(TCallable&& callable)
	{
		if constexpr (Index < Size) {
			using Container_t = typename std::tuple_element<Index, ContainersTuple_t>::type;
			using Object_t = typename Container_t::value_type;
			
			if constexpr (HasComponents<Object_t, TComponent>::value) {
				for (auto& object : this->getContainer<Object_t>()) {
					std::invoke(callable, object);
				}
			}

			if constexpr (Index + 1 < Size) {
				forEach<TComponent, Index + 1>(std::forward<ContainersTuple_t>(containersTuple), std::forward<TCallable>(callable));
			}
		}
	}
};

}

using Componable::Object;
using Componable::ObjectsHolder;
using Componable::Component;
using Componable::HasComponents;

}
