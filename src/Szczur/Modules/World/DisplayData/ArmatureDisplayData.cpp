#include "ArmatureDisplayData.hpp"

/** @file ArmatureDisplayData.cpp
 ** @description Implementaion with armature display data class as shared game armature display data.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski
 **/

#include <string>
#include <exception>
#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>

#include <dragonBones/SFMLArmatureDisplay.h>
#include <dragonBones/model/DragonBonesData.h>
#include <SFML/Graphics/Texture.hpp>
#include <dragonBones/model/TextureAtlasData.h>
#include "json.hpp"
using json = nlohmann::json;

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{

/* Properties */
/// Factory
DragonBones::Factory_t& DragonBones::getFactory()
{
	return this->factory;
}
const DragonBones::Factory_t& DragonBones::getFactory() const
{
	return this->factory;
}



/* Operators */
/// Constructor 
ArmatureDisplayData::ArmatureDisplayData(const std::string& name, DragonBones::Factory_t& factory)
:
	factory(factory)
{
	this->name = name;
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loading `", this->name, "` armature display data.");

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
		std::throw_with_nested(std::runtime_error("Could not load armature display data: `" + this->name + "`."));
	}
	
	// @todo , togglable states? (grouped textures and visibility switching)?
	
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loaded armature display data with " + std::to_string(this->textures.size()) + " textures and atlases.");
}

}
