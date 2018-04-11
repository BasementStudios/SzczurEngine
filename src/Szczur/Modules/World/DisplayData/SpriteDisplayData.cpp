#include "SpriteDisplayData.hpp"

/** @file SpriteDisplayData.cpp
 ** @description Implementaion with sprite display data class as shared game sprite display data.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski
 **/

#include <string>
#include <exception>
#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/Vertex.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{

/* Properties */
/// Texture
const sf3d::Texture& SpriteDisplayData::getTexture() const
{
	return this->texture;
}

/// Vertices
const sf3d::VertexArray& SpriteDisplayData::getVertices() const
{
	return this->verticles;
}



/* Operators */
/// Constructor 
SpriteDisplayData::SpriteDisplayData(const std::string& name)
{
	this->name = name;
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loading `", name, "` sprite display data.");

	try {
		// Load texture file
		if (!this->texture.loadFromFile(SpriteDisplayData::assetsFolderPath + this->name + SpriteDisplayData::textureFilePath)) {
			throw std::runtime_error("Cannot load texture file.");
		}
		
		// @todo ? config verticles/origin
		
		// Setup verticles
		auto& size = this->texture.getSize();
		this->verticles.set(0u, {
			{0.f, 0.f, 0.f},
			{1.f, 1.f, 1.f},
			{0.f, 0.f}
		});
		this->verticles.set(1u, {
			{(float)size.x, 0.f, 0.f},
			{1.f, 1.f, 1.f},
			{1.f, 0.f}
		});
		this->verticles.set(2u, {
			{(float)size.x, -(float)size.y, 0.f},
			{1.f, 1.f, 1.f},
			{1.f, 1.f}
		});
		this->verticles.set(3u, {
			{0.f, -(float)size.y, 0.f},
			{1.f, 1.f, 1.f},
			{0.f, 1.f}
		});
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Could not load sprite display data: `" + this->name + "`."));
	}
	
	LOG_WARNING("{*", this, "\"", this->name, "} ", "Sprite display data loaded.");
}

}
