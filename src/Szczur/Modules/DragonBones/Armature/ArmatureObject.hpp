#pragma once

/** @file ArmatureObject.hpp
 ** @description Header file with armature object class.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

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
	/* Fields */
protected: 
	ArmatureObjectType::PosesContainer_t<ArmatureObjectType::Pose_t*> poses;
	ArmatureObjectType::PoseID_t currentPoseID {ArmatureObjectType::defaultPoseID};
	
	
	
	/* Properties */
public:
	/** @property ArmatureType
     ** @description Class/type of the object.
	 ** @access const get
	 **/
	const ArmatureObjectType* getArmatureType() const;

	/** @property CurrentPose
	 ** @description Current displayed pose.
	 ** @access pointer get, string get set
	 **/
	ArmatureObjectType::Pose_t* getCurrentPose();
	const ArmatureObjectType::Pose_t* getCurrentPose() const;
	void setCurrentPose(ArmatureObjectType::PoseID_t poseID);
	void setCurrentPose(const std::string& poseString);
	const std::string& getCurrentPoseString() const;

	/** @property Animation 
	 ** @description Provides access to DragonBones Animation object of current pose.
	 ** @access pointer get
	 **/
	dragonBones::Animation* getAnimation();



	/* Operators */
public:
	ArmatureObject(
		const ArmatureObjectType* 			type, 
		const std::string& 					name, 
		const Object::Vector_t&				position	= {},
		const Object::Vector_t&				speed 		= {},
		const ArmatureObjectType::PoseID_t&	poseID		= ArmatureObjectType::defaultPoseID
	);
	/// Standard constructor
	ArmatureObject(
		const ArmatureObjectType* 		type, 
		const std::string& 				name, 
		const Object::Vector_t& 		position,
		const std::string&				poseString,
		const std::string&				animation 	= "",
		const Object::Vector_t&			speed 		= {}
	);



	/* Methods */
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

}
