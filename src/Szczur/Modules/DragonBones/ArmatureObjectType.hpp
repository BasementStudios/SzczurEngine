#pragma once

/** @file ArmatureObjectType.hpp
 ** @description Header file with Armature Object Type class as shared game armature object type information.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>
#include <string>

#include <dragonBones/SFMLArmatureDisplay.h>
#include <dragonBones/model/DragonBonesData.h>
#include <dragonBones/model/TextureAtlasData.h>

#include <SFML/Graphics/Texture.hpp>

#include "Szczur/Modules/DragonBones/DragonBones.hpp"
#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"

namespace rat
{

class ArmatureObjectType : public ObjectType
{
	/* Types */
public:
	using Pose_t 			= dragonBones::SFMLArmatureDisplay;
	template <typename T>
	using PosesContainer_t 	= std::vector<T>;

	using PoseID_t 			= PosesContainer_t<Pose_t>::size_type;


	
	/* Constants */
public:
	constexpr static PoseID_t defaultPoseID = 0;

protected:
	constexpr static auto	armaturesAssetsPath	= "Assets/Objects/";
	constexpr static auto	skeletonDataPath	= "/skeleton.json";
	constexpr static auto 	texturesPathPrefix	= "/texture_";
	constexpr static auto 	texturesPathSuffix	= ".png";
	
	
	
	/* Fields */
protected:
	rat::DragonBones::Factory_t&				factory;
	dragonBones::DragonBonesData*				skeletonData;
	std::vector<sf::Texture>					textures; // @todo . dynarray instead of vector
	std::vector<dragonBones::SFMLTextureData*>	texturesData; // @todo . dynarray instead of vector
	dragonBones::TextureAtlasData*				atlasData;



	/* Properties */
public:
	/** @property Pose string/ID
	 ** @description Pose string/ID used to identify poses.
	 ** @acess const get
	 **/
	const std::string& getPoseString(ArmatureObjectType::PoseID_t poseID) const;
	ArmatureObjectType::PoseID_t getPoseID(const std::string& poseString) const;

	const std::vector<std::string>& getPosesNames() const;



	/* Operators */
public:
	/** @constructor
	 ** @description Loads armature object type.
	 ** @argument name - name of the armature object type to load.
	 **/
	ArmatureObjectType(const std::string& name, rat::DragonBones::Factory_t& factory);

	/** @destructor
	 ** @description Unloads data used by armature object poses.
	 **/
	~ArmatureObjectType();

	// Disable coping
	ArmatureObjectType(const ArmatureObjectType&) = delete;
	ArmatureObjectType& operator = (const ArmatureObjectType&) = delete;

	// Disable moving
	ArmatureObjectType(ArmatureObjectType&&) = delete;
	ArmatureObjectType& operator = (ArmatureObjectType&&) = delete;
	


	/* Methods */
public:
	/// Creates new poses of this type
	PosesContainer_t<Pose_t*> createPoses() const;
};

}
