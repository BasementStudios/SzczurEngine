#include "ArmatureObjectType.hpp"

/** @file ArmatureObjectType.hpp
 ** @description Implementation file with Armature Object Type class as shared game armature object type information.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <vector>

#include <fstream>

#include "Szczur/JSON.hpp"
#include "Szczur/Debug/Logger.hpp"
#include "Szczur/Modules/DragonBones/DragonBones.hpp"

namespace rat
{

/* Operators */
/// Constructor
ArmatureObjectType::ArmatureObjectType(const std::string& typeName, rat::DragonBones::Factory_t& factory)
	: factory(factory)
{
	// Open config file
	std::ifstream file(ArmatureObjectType::assetsPath + typeName + '/' + ArmatureObjectType::configFileName);
	if (!file.is_open()) {
		LOG_ERROR("[World][ArmatureObjectType{*", this, ";\"", this->name, "}] ", 
			"Cannot load config file!");
		return; // @todo exception?
	}

	try {
		// Load JSON 
		json configJSON;
		file >> configJSON;

		auto& posesJSON = configJSON["poses"];
		auto posesJSONIt = posesJSON.begin(); 
		this->posesDetails.resize(posesJSON.size());
		auto posesDetailsIt = this->posesDetails.begin();

		// Load poses
		while (posesJSONIt != posesJSON.end()) {
			std::string poseName = posesJSONIt.key();

			// Load texture file for the pose
			if (!(posesDetailsIt->texture.loadFromFile(ArmatureObjectType::assetsPath + typeName + '/' + ArmatureObjectType::textureFileNamePrefix + poseName +  ArmatureObjectType::textureFileNameSuffix))) {
				LOG_ERROR("[World][ArmatureObjectType{*", this, ";\"", this->name, "}] ", 
					"Cannot load texture file for ", poseName, " pose!");
				return; // @todo exception?
			}

			// Load texture atlas
			if (!(posesDetailsIt->atlasData = this->factory.loadTextureAtlasData(ArmatureObjectType::assetsPath + typeName + '/' + ArmatureObjectType::textureAtlasDataFileNamePrefix + poseName + ArmatureObjectType::textureAtlasDataFileNameSuffix, &(posesDetailsIt->texture)))) {
				LOG_ERROR("[World][ArmatureObjectType{*", this, ";\"", this->name, "}] ", 
					"Cannot load texture atlas for ", poseName, " pose!");
				return; // @todo exception?
			}

			// Load skeleton
			if (!(posesDetailsIt->skeletonData = this->factory.loadDragonBonesData(ArmatureObjectType::assetsPath + typeName + '/' + ArmatureObjectType::dragonBonesDataFileNamePrefix + poseName + ArmatureObjectType::dragonBonesDataFileNameSuffix))) {
				LOG_ERROR("[World][ArmatureObjectType{*", this, ";\"", this->name, "}] ", 
					"Cannot load skeleton data for ", poseName, " pose!");
				return; // @todo exception?
			}

			// @todo , togglable states? (grouped textures and visibility switching)?
			//auto& detailsJSON = posesJSONIt.value();

			// Next pose config
			++posesJSONIt;
			++posesDetailsIt;
		}
	}
	catch(std::exception& e) {
		LOG_ERROR("[World][ArmatureObjectType{*", this, ";\"", this->name, "}] ", 
			"Error while parsing configuration file! Exception message: ", e.what());
		return; // @todo exception?
	}
	
	LOG_INFO("[World][ArmatureObjectType{*", this, ";\"", this->name, "}] ", 
		"Loaded with ", posesDetails.size(), " states.");
}

ArmatureObjectType::~ArmatureObjectType()
{
	for (auto& pose : this->posesDetails) {
		this->factory.removeTextureAtlasData(pose.atlasData->name);
		this->factory.removeDragonBonesData(pose.skeletonData->name);
	}
}

}
