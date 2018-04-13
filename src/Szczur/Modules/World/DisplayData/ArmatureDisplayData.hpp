#pragma once

/** @file ArmatureDisplayData.hpp
 ** @description Header file with armature display data class as shared game armature display data.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <vector>

#include <dragonBones/model/DragonBonesData.h>
#include <SFML/Graphics/Texture.hpp>
#include <dragonBones/model/TextureAtlasData.h>

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"

#include "Szczur/Modules/DragonBones/DragonBones.hpp" // Factory_t

namespace rat
{

class ArmatureDisplayData
{
	/* Types */
public:
	using SkeletonData_t 	= dragonBones::DragonBonesData;
	using Texture_t    		= sf::Texture;
	using AtlasData_t		= dragonBones::TextureAtlasData;
	


	/* Constants */
protected:
	// @info Data paths templates
	// 	Colon (`:`) stands for armature type name and question mark (`?`) for next numbers,
	//	starting from 0, since it's common that there can be more than one atlas and texture.
	// 	At least one texture with its atlas must be loaded for the armature, the number 0.
	constexpr static auto	skeletonDataPathTemplate = "Assets/Objects/:/:_ske.json";
	constexpr static auto	atlasTexturePathTemplate = "Assets/Objects/:/:_tex_?.png";
	constexpr static auto	atlasDataPathTemplate    = "Assets/Objects/:/:_tex_?.json";
	

	
	/* Fields */
public: // @todo prot&prop
	std::string			name;
protected:
	DragonBones::Factory_t&         factory;
	SkeletonData_t*					skeletonData;
	std::vector<Texture_t>			textures;
	std::vector<AtlasData_t*>		atlasesData;



	/* Properties */
public:
	/** @property Factory
	 ** @description Provides access to armature factory.
	 ** @access referecne get
	 **/
	DragonBones::Factory_t& getFactory();
	const DragonBones::Factory_t& getFactory() const;
	
	
	
	/* Operators */
public:
	/** @constructor
	 ** @description Loads armature display data.
	 ** @argument name - name of the armature display data to load.
	 **/
	ArmatureDisplayData(const std::string& name, DragonBones::Factory_t& factory);

	// Disable coping
	//ArmatureDisplayData(const ArmatureDisplayData&) = delete; // @warn ? @todo .
	//ArmatureDisplayData& operator = (const ArmatureDisplayData&) = delete;

	// Disable moving
	//ArmatureDisplayData(ArmatureDisplayData&&) = delete;
	//ArmatureDisplayData& operator = (ArmatureDisplayData&&) = delete;
};

}
