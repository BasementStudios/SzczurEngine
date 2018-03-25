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
#include <SFML/Graphics/Texture.hpp>
#include <dragonBones/model/TextureAtlasData.h>

#include "Szczur/Modules/DragonBones/DragonBones.hpp"
#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"

namespace rat
{

/** @class ArmatureObjectType
 ** @inherit ObjectType
 ** @description Load and contains shared data used to render Armature Objects.
 **/
class ArmatureObjectType : public ObjectType
{
	/* Types */
public:
	using Pose_t 			= dragonBones::SFMLArmatureDisplay;
	template <typename T>
	using PosesContainer_t 	= std::vector<T>;
	
	using PoseID_t 			= PosesContainer_t<Pose_t>::size_type;

	using SkeletonData_t 	= dragonBones::DragonBonesData;
	using Texture_t    		= sf::Texture;
	using AtlasData_t		= dragonBones::TextureAtlasData;


	
	/* Constants */
public:
	constexpr static PoseID_t defaultPoseID = 0;

protected:
	// @info Data paths
	// 	Colon (`:`) stands for armature type name and question mark (`?`) for next numbers,
	//	starting from 0, since it's common that there can be more than one atlas and texture.
	// 	At least one texture with its atlas must be loaded for the armature, the number 0.
	constexpr static auto	skeletonDataPathTemplate = "Assets/Objects/:/:_ske.json";
	constexpr static auto	atlasTexturePathTemplate = "Assets/Objects/:/:_tex_?.png";
	constexpr static auto	atlasDataPathTemplate    = "Assets/Objects/:/:_tex_?.json";
	//constexpr static auto	skeletonDataPathTemplate = "Assets/Objects/:/skeleton.json";
	//constexpr static auto	atlasTexturePathTemplate = "Assets/Objects/:/texture_?.png";
	//constexpr static auto	atlasDataPathTemplate    = "Assets/Objects/:/atlas_?.json";

	
	
	/* Fields */
protected:
	rat::DragonBones::Factory_t&	factory;
	SkeletonData_t*					skeletonData;
	std::vector<Texture_t>			textures;
	std::vector<AtlasData_t*>		atlasesData;



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
