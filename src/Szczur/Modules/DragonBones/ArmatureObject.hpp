#pragma once

/** @file ArmatureObject.hpp
 ** @description Header file with armature object class.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <dragonBones/armature/IArmatureProxy.h>

#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"
#include "ArmatureObjectType.hpp"

namespace rat
{

/** @class ArmatureObject
 ** @implements Object
 **/
class ArmatureObject : public Object
{
	/* Types */
public:
	using Pose_t 			= dragonBones::Armature*;
	
	using PoseID_t 			= std::size_t;
	
	template <typename... Ts>
	using PoseContainer_t 	= std::vector<Ts...>;
	
	

	/* Fields */
protected: 
	PoseContainer_t<Pose_t>	poses; // @note trzymac wszstkie vs current only
	

	
	
	/* Properties */
public:
	/** @property 
	 ** @description
	 ** @access get 
	 **/
	



	/* Operators */
public:
	ArmatureObject(
		const ArmatureObjectType*	type, 
		const std::string& 			name, 
		Object::Vector_t 			position 	= {0.f, 0.f}, 
		Object::Vector_t 			speed 		= {0.f, 0.f}
		// @todo . starting pose, animation
	);



	/* Methods */
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

}
