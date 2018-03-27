#include "ArmatureObjectType.hpp"

/** @file ArmatureObjectType.hpp
 ** @description Implementation file with Armature Object Type class as shared game armature object type information.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <exception>
#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>
#include <filesystem>

#include <dragonBones/SFMLArmatureDisplay.h>
#include <dragonBones/model/DragonBonesData.h>
#include <SFML/Graphics/Texture.hpp>
#include <dragonBones/model/TextureAtlasData.h>
#include "json.hpp"

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/DragonBones/DragonBones.hpp"

namespace rat
{

/* Local inline function */
namespace local {

inline std::string stringReplaced(const std::string& in, const std::string& from, const std::string& to)
{
	std::string string = in;
	
	for (
		std::size_t i = string.find(from); 
		i != std::string::npos; 
		i = string.find(from, i)
	) {
		string.replace(i, from.length(), to);
	}

	return string;
}

}



/* Properties */ 
const std::string& ArmatureObjectType::getPoseString(ArmatureObjectType::PoseID_t poseID) const
{
	const auto& names = this->getPosesNames();
	if (names.size() > poseID) {
		return names[poseID];
	}
	LOG_WARNING("{*", this, "\"", this->name, "} ", "Pose of ID `", poseID, "` was not found; using default.");
	return names[ArmatureObjectType::defaultPoseID];
}
ArmatureObjectType::PoseID_t ArmatureObjectType::getPoseID(const std::string& poseString) const
{
	const auto& names = this->getPosesNames();
	for (std::vector<std::string>::size_type i = 0, I = names.size(); i < I; i++) {
        if (names[i] == poseString) {
            return i;
        }
    }
    LOG_WARNING("{*", this, "\"", this->name, "} ", "Pose of ID for name `", poseString, "` was not found; using default.");
	return ArmatureObjectType::defaultPoseID;
}

const std::vector<std::string>& ArmatureObjectType::getPosesNames() const
{
	return this->skeletonData->armatureNames;
}



/* Operators */
/// Constructor
ArmatureObjectType::ArmatureObjectType(const std::string& typeName, rat::DragonBones::Factory_t& factory)
	: factory(factory)
{
	this->name = typeName;
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loading `", typeName, "` armature object type.");
	
	try {
		// Load skeleton data
		try {
			// Find file
			std::string skeletonDataPath = local::stringReplaced(ArmatureObjectType::skeletonDataPathTemplate, ":", this->name);

			if (!std::filesystem::exists(skeletonDataPath)) {
				throw std::runtime_error("Armature skeleton data file doesn't exist.");
			}

			// Load the skeleton data
			LOG_INFO("{*", this, "\"", this->name, "} ", "Loading skeleton data from `" + skeletonDataPath + "`.");
			this->skeletonData = this->factory.loadDragonBonesData(skeletonDataPath);
			
			if (this->skeletonData == nullptr) {
				throw std::runtime_error("Armature skeleton data is invaild.");
			}
		}
		catch (...) {
			std::throw_with_nested(std::runtime_error("Could not load armature skeleton data."));
		}

		// Load textures and atlas 
		try {
			// Resolve common parts of path
			std::string atlasTexturePathCommon = local::stringReplaced(ArmatureObjectType::atlasTexturePathTemplate, ":", this->name);
			std::string atlasDataPathCommon = local::stringReplaced(ArmatureObjectType::atlasDataPathTemplate, ":", this->name);
			
			// Count atlases
			std::size_t atlasCount = 0;
			while (true) {
				// Find next atlas path
				std::string atlasDataPath = local::stringReplaced(atlasDataPathCommon, "?", std::to_string(atlasCount));
				
				if (!std::filesystem::exists(atlasDataPath)) {
					break;
				}

				atlasCount += 1;
			}

			// Check is there at least on texture
			LOG_INFO("{*", this, "\"", this->name, "} ", "Found " + std::to_string(atlasCount) + " atlas data files.");
			if (atlasCount < 1) {
				throw std::runtime_error("Armature atlas number 0 doesn't exist.");
			}

			// Allocate atlas textures and atlas data containers
			this->atlasesData.reserve(atlasCount);
			this->textures.reserve(atlasCount);
			
			// Load atlases textures and data
			for (std::size_t atlasNumber = 0; atlasNumber < atlasCount; ++atlasNumber) {
				// Find atlas texture
				std::string atlasTexturePath = local::stringReplaced(atlasTexturePathCommon, "?", std::to_string(atlasNumber));

				if (!std::filesystem::exists(atlasTexturePath)) {
					throw std::runtime_error("Armature atlas texture file number " + std::to_string(atlasNumber) + " doesn't exist.");
				}

				// Find atlas data
				std::string atlasDataPath = local::stringReplaced(atlasDataPathCommon, "?", std::to_string(atlasNumber));

				// Load atlas texture
				LOG_INFO("{*", this, "\"", this->name, "} ",	"Loading atlas texture: `" + atlasTexturePath + "`.");
				auto& texture = this->textures.emplace_back();
				if (!texture.loadFromFile(atlasTexturePath)) {
					throw std::runtime_error("Could not load texture: `" + atlasTexturePath + "`.");
				}

				// Load atlas data
				LOG_INFO("{*", this, "\"", this->name, "} ", "Loading atlas data: `" + atlasDataPath + "`.");
				auto* data = this->factory.loadTextureAtlasData(atlasDataPath, &texture);
				this->atlasesData.push_back(data);
			}
		}
		catch(...) {
			std::throw_with_nested(std::runtime_error("Could not load textures."));
		}
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Could not load armature object type: `" + this->name + "`."));
	}
	
	// @todo , togglable states? (grouped textures and visibility switching)?
	
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loaded " + std::to_string(this->getPosesNames().size()) + " armature poses and " + std::to_string(this->textures.size()) + " textures with atlases.");
}

ArmatureObjectType::~ArmatureObjectType()
{
	this->factory.removeDragonBonesData(this->skeletonData->name);
	for (auto& atlasData : this->atlasesData) {
		this->factory.removeTextureAtlasData(atlasData->name);
	}
}



/* Methods */
/// createPose
ArmatureObjectType::PosesContainer_t<ArmatureObjectType::Pose_t*> ArmatureObjectType::createPoses() const
{
	ArmatureObjectType::PosesContainer_t<ArmatureObjectType::Pose_t*> poses;
	poses.reserve(this->getPosesNames().size());
	
	for (const auto& name : this->getPosesNames()) {
		poses.push_back(this->factory.buildArmatureDisplay(name));
	}
	
	return poses;
}

}
