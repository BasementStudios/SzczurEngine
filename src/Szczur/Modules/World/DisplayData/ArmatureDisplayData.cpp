#include "ArmatureDisplayData.hpp"

/** @file ArmatureDisplayData.cpp
 ** @description Implementaion with armature display data class as shared game armature display data.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski
 ** @music Taconafide - SOMA
 **/

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <limits>

#include <dragonBones/model/DragonBonesData.h> // @todo , FWD
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include <Szczur/Modules/DragonBones/SF3DTextureAtlasData.hpp>
#include <Szczur/Modules/DragonBones/SF3DTextureData.hpp>

#include "Szczur/Modules/DragonBones/DragonBones.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{

/* Properties */
dragonBones::SF3DTextureData* ArmatureDisplayData::getTextureData(const std::string& name) const
{
	for (const auto textureAtlasData : atlasesData) {
		const auto textureData = textureAtlasData->getTexture(name);
		if (textureData != nullptr) {
			return static_cast<dragonBones::SF3DTextureData*>(textureData);
		}
	}
	
	LOG_WARN("{*", this, "\"", this->name, "} ", "Could not find texture data: `", name, "`.");
	return nullptr;
}



/* Operators */
/// Constructor 
ArmatureDisplayData::ArmatureDisplayData(const std::string& name, DragonBones& dragonBonesModule)
{
	this->name = name;
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loading `", this->name, "` armature display data.");

	try {
		this->loadSkeletonData(name, dragonBonesModule);
		this->loadTextures(name, dragonBonesModule);
		// @todo , togglable states? (grouped textures and visibility switching)?
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Could not load armature display data: `" + this->name + "`."));
	}
	
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loaded armature display data with " + std::to_string(this->textures.size()) + " textures and atlases.");
}



/* Submethods */
void ArmatureDisplayData::loadSkeletonData(const std::string& name, DragonBones& dragonBonesModule)
{
	try {
		// Find file
		std::string path = stringReplaced(ArmatureObjectType::skeletonDataPathTemplate, ":", this->name);

		if (!std::filesystem::exists(path)) {
			throw std::runtime_error("Armature skeleton data file doesn't exist.");
		}
		LOG_INFO("{*", this, "\"", this->name, "} ", "Loading skeleton data from `" + path + "`.");

		// Load file
		std::ifstream file(path);
		
		try {
			// Get data length
			file.seekg(0, std::ios::end);
			file.ignore(std::numeric_limits<std::streamsize>::max());
			std::size_t size = file.gcount();
			file.clear();
			file.seekg(0, std::ios_base::beg);

			// Get file content to buffer
			std::string buffer;
			buffer.reserve(size + 1);
			buffer[size] = '\0';
			file.read(&buffer[0], size);
			
			// Parse the data in buffer
			dragonBones::DataParser& dataParser = nullptr;
			if (
				rawData[0] == 'D' &&
				rawData[1] == 'B' &&
				rawData[2] == 'D' &&
				rawData[3] == 'T'
			) {
				dataParser = dragonBonesModule.binaryParser;
			}
			else {
				dataParser = dragonBonesModule.jsonParser;
			}
			this->skeletonData = dataParser.parseDragonBonesData(buffer.c_str(), name, 1.f);
		}
		catch (...) {
			std::throw_with_nested(std::runtime_error("Could not parse armature skeleton data file."));
		}
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Could not load armature skeleton data."));
	}
}

void ArmatureDisplayData::loadTextures(const std::string& name, DragonBones& dragonBonesModule)
{
	// Load textures and atlas 
	try {
		// Resolve common parts of path
		std::string atlasTexturePathCommon = stringReplaced(ArmatureObjectType::atlasTexturePathTemplate, ":", this->name);
		std::string atlasDataPathCommon = stringReplaced(ArmatureObjectType::atlasDataPathTemplate, ":", this->name);
		
		// Count atlases
		std::size_t atlasCount = 0;
		while (true) {
			// Find next atlas path
			std::string atlasDataPath = stringReplaced(atlasDataPathCommon, "?", std::to_string(atlasCount));
			
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

		// Allocate atlas textures container and resize atlas data container.
		// @info It's highly neccessary to preallocate for the textures
		this->atlasesData.resize(atlasCount);
		this->textures.reserve(atlasCount);
		
		// Load atlases textures and data
		for (std::size_t atlasNumber = 0; atlasNumber < atlasCount; ++atlasNumber) {
			// Load textures
			LOG_INFO("{*", this, "\"", this->name, "} ", "Loading atlas texture: `" + atlasTexturePath + "`.");
			try {
				// Find atlas texture
				std::string atlasTexturePath = stringReplaced(atlasTexturePathCommon, "?", std::to_string(atlasNumber));

				if (!std::filesystem::exists(atlasTexturePath)) {
					throw std::runtime_error("Armature atlas texture file number " + std::to_string(atlasNumber) + " doesn't exist.");
				}

				// Find atlas data
				std::string atlasDataPath = stringReplaced(atlasDataPathCommon, "?", std::to_string(atlasNumber));

				// Load atlas texture
				auto& texture = this->textures.emplace_back();
				if (!texture.loadFromFile(atlasTexturePath)) {
					throw std::runtime_error("Could not load texture: `" + atlasTexturePath + "`.");
				}
			}
			catch(...) {
				std::throw_with_nested(std::runtime_error("Could not load armature atlas data."));
			}

			// Load atlas data
			LOG_INFO("{*", this, "\"", this->name, "} ", "Loading atlas data: `" + atlasDataPath + "`.");
			{
				// Load file
				std::ifstream file(path);

				try {
					// Get data length
					file.seekg(0, std::ios::end);
					file.ignore(std::numeric_limits<std::streamsize>::max());
					std::size_t size = file.gcount();
					file.clear();
					file.seekg(0, std::ios_base::beg);

					// Get file content to buffer
					std::string buffer;
					buffer.reserve(size + 1);
					buffer[size] = '\0';
					file.read(&buffer[0], size);
					
					// Parse the data in buffer
					dragonBones::DataParser& dataParser = nullptr;
					if (
						rawData[0] == 'D' &&
						rawData[1] == 'B' &&
						rawData[2] == 'D' &&
						rawData[3] == 'T' // @warn There is no binary format for texture atlas data in DragonBones (5.6)
					) {
						dataParser = dragonBonesModule.binaryParser;
					}
					else {
						dataParser = dragonBonesModule.jsonParser;
					}
					this->atlasesData[atlasNumber] = dataParser.parseTextureAtlasData(buffer.c_str(), this->atlasesData[atlasNumber], name, 1.f);
					
					// Setup atlas data with loaded texture
					this->atlasesData[atlasNumber].setRenderTexture(this->textures[atlasNumber]);
				}
				catch (...) {
					std::throw_with_nested(std::runtime_error("Could not parse armature atlas data."));
				}
			}
		}
	}
	catch(...) {
		std::throw_with_nested(std::runtime_error("Could not load textures."));
	}
}

}
