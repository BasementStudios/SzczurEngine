#pragma once

/** @file ArmatureObjectType.hpp
 ** @description Header file with Armature Object Type class as shared game armature object type information.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

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
	template <typename... Ts>
	using PoseDetailsContainer_t = std::vector<Ts...>;

	struct PoseDetails
	{
		dragonBones::DragonBonesData* 	skeletonData = nullptr;
		sf::Texture 					texture; // @todo ? elements from separated files? (due to universal models concept)
		dragonBones::TextureAtlasData* 	atlasData = nullptr;
		// @todo , togglable states? (grouped textures and visibility switching)?
	};


	
	/* Constants */
protected:
	constexpr static auto	assetsPath		= "Assets/Objects/";
	constexpr static auto	configFileName	= "object.json";
	constexpr static auto 	textureFileNamePrefix			= "texture_";
	constexpr static auto 	textureFileNameSuffix			= ".png";
	constexpr static auto	dragonBonesDataFileNamePrefix	= "skeleton_";
	constexpr static auto	dragonBonesDataFileNameSuffix	= ".json";
	constexpr static auto	textureAtlasDataFileNamePrefix	= "texatlas_";
	constexpr static auto	textureAtlasDataFileNameSuffix	= ".json";
	
	
	
	/* Fields */
protected:
	rat::DragonBones::Factory_t&		factory;
	PoseDetailsContainer_t<PoseDetails>	posesDetails;



	/* Properties */
public:
	// @info There is no properties since Armature uses DragonBones to almost whole rendering process.
	// @todo ! getPoseXYZ via name/id



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
};

}
