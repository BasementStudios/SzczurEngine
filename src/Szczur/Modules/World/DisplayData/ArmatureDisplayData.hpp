#pragma once

/** @file ArmatureDisplayData.hpp
 ** @description Header file with armature display data class as shared game armature display data.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @music Taconafide - SOMA
 **/

#include <string>
#include <vector>

#include <dragonBones/model/DragonBonesData.h> // @todo , FWD
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include <Szczur/Modules/DragonBones/SF3DTextureAtlasData.hpp>
#include <Szczur/Modules/DragonBones/SF3DTextureData.hpp>

#include "Szczur/Modules/DragonBones/DragonBones.hpp"

namespace rat
{

class ArmatureDisplayData
{
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
	std::string name;
protected:
	dragonBones::DragonBonesData*                  skeletonData; // @info Pointer, because created by DB parsers...
	std::vector<sf3d::Texture>                     textures;
	std::vector<dragonBones::SF3DTextureAtlasData> atlasesData;



	/* Properties */
public:
	/** @property TextureData
	 ** @description Details of texture in DragonBones texture atlas.
	 ** @access pointer get // @todo , should be const 
	 **/
	dragonBones::SF3DTextureData* getTextureData(const std::string& name) const;
	
	
	
	/* Operators */
public:
	/** @constructor
	 ** @description Loads armature display data.
	 ** @argument name - name of the armature display data to load.
	 **/
	ArmatureDisplayData(const std::string& name, DragonBones& dragonBonesModule);

	// Disable coping
	//ArmatureDisplayData(const ArmatureDisplayData&) = delete; // @warn ? @todo .
	//ArmatureDisplayData& operator = (const ArmatureDisplayData&) = delete;

	// Disable moving
	//ArmatureDisplayData(ArmatureDisplayData&&) = delete;
	//ArmatureDisplayData& operator = (ArmatureDisplayData&&) = delete;

	
	
	/* Submethods */
protected:
	loadSkeletonData(const std::string& name, DragonBones& dragonBonesModule);
	loadTextures(const std::string& name, DragonBones& dragonBonesModule);
};

}
