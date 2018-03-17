#include "ArmatureObject.hpp"

/** @file ArmatureObject.cpp
 ** @description Implementation file with armature object class.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <dragonBones/SFMLDisplay.h>

#include "Szczur/Modules/World/ObjectType.hpp"
#include "Szczur/Modules/World/Object.hpp"
#include "ArmatureObjectType.hpp"

namespace rat
{

/* Properties */
/// ArmatureType
const ArmatureObjectType* ArmatureObject::getArmatureType() const
{
	return reinterpret_cast<const ArmatureObjectType*>(this->type);
}

/// CurrentPose
ArmatureObjectType::Pose_t* ArmatureObject::getCurrentPose()
{
	return this->poses[this->currentPoseID];
}
const ArmatureObjectType::Pose_t* ArmatureObject::getCurrentPose() const
{
	return this->poses[this->currentPoseID];
}
void ArmatureObject::setCurrentPose(ArmatureObjectType::PoseID_t poseID)
{
	this->currentPoseID = poseID;
}
void ArmatureObject::setCurrentPose(const std::string& poseString)
{
	this->currentPoseID = this->getArmatureType()->getPoseID(poseString);
}
const std::string& ArmatureObject::getCurrentPoseString() const
{
	return this->getArmatureType()->getPoseString(this->currentPoseID);
}



/* Operators */
/// Full constructor
ArmatureObject::ArmatureObject(
	const ArmatureObjectType* 			type, 
	const std::string& 					name, 
	const Object::Vector_t&				position,
	const Object::Vector_t&				speed,
	const ArmatureObjectType::PoseID_t&	poseID
)
	: Object::Object((ObjectType*)type, name, position, speed)
{
	this->poses = this->getArmatureType()->createPoses();
}
/// Standard constructor
ArmatureObject::ArmatureObject(
	const ArmatureObjectType* 		type, 
	const std::string& 				name, 
	const Object::Vector_t& 		position,
	const std::string&				poseString,
	const Object::Vector_t&			speed
)
	: Object::Object((ObjectType*)type, name, position, speed)
{
	this->poses = this->getArmatureType()->createPoses();
}



/* Methods */
/// draw
void ArmatureObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(this->position);
	target.draw(*(this->getCurrentPose()), states);
}

}
