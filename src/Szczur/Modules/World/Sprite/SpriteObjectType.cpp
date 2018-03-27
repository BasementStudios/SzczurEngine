#include "SpriteObjectType.hpp"

/** @file SpriteObjectType.cpp
 ** @description Implementaion with Sprite Object Type class as shared game sprite object type information.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <exception>
#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include "json.hpp"
using json = nlohmann::json;

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"

namespace rat
{

/* Properties */
/// Texture
const sf::Texture& SpriteObjectType::getTexture() const
{
	return this->texture;
}

/// States
const std::string& SpriteObjectType::getStateString(const SpriteObjectType::StateID_t& stateID) const
{
	if (this->statesDetails.size() > stateID) {
		return this->statesDetails[stateID].name;
	}
	// Fallback
	LOG_WARNING("{*", this, "\"", this->name, "} ", "State of ID `", stateID, "` was not found; using default.");
	return this->statesDetails[SpriteObjectType::defaultStateID].name;
}
SpriteObjectType::StateID_t SpriteObjectType::getStateID(const std::string& stateString) const
{
    for (SpriteObjectType::StateID_t i = 0, I = this->statesDetails.size(); i != I; i++) {
        if (this->statesDetails[i].name == stateString) {
            return i;
        }
    }
	// Fallback
    LOG_WARNING("{*", this, "\"", this->name, "} ", "State of ID for name `", stateString, "` was not found; using default.");
	return SpriteObjectType::defaultStateID;
}

/// Origin
const Object::Vector_t SpriteObjectType::getOrigin(const SpriteObjectType::StateID_t& stateID) const
{
	if (this->statesDetails.size() > stateID) {
		return this->statesDetails[stateID].origin;
	}
	// Fallback
	LOG_WARNING("{*", this, "\"", this->name, "} ", "Origin for state ID `", stateID, "` was not found; using default.")
	return this->statesDetails[SpriteObjectType::defaultStateID].origin;
}

/// Vertices
const std::array<sf::Vertex, 4> SpriteObjectType::getVertices(const SpriteObjectType::StateID_t& stateID) const
{
	if (this->statesDetails.size() > stateID) {
		return this->statesDetails[stateID].vertices;
	}
	// Fallback
	LOG_WARNING("{*", this, "\"", this->name, "} ", "Vertices for state ID `", stateID, "` was not found; using default.")
	return this->statesDetails[SpriteObjectType::defaultStateID].vertices;
}



/* Operators */
/// Constructor 
SpriteObjectType::SpriteObjectType(const std::string& typeName)
{	
	this->name = typeName;
	LOG_INFO("{*", this, "\"", this->name, "} ", "Loading `", typeName, "` sprite object type.");

	try {
		// Load texture file
		if (!this->texture.loadFromFile(SpriteObjectType::spritesAssetsPath + this->name + SpriteObjectType::textureFilePath)) {
			throw std::runtime_error("Cannot load texture file.");
		}
		
		// Open config file
		std::ifstream configFile(SpriteObjectType::spritesAssetsPath + this->name + SpriteObjectType::configFilePath);
		json configJSON;
		configFile >> configJSON;

		// Counting states to dive texture to sprite frames
		std::size_t statesCount = 0;

		// Load states
		auto& statesJSON = configJSON["states"];
		this->statesDetails.reserve(statesJSON.size());
		
		for (auto statesIt = statesJSON.begin(); statesIt != statesJSON.end(); ++statesIt) {
			auto& detailsJSON = statesIt.value();
			
			// Load origin
			Object::Vector_t origin {0, 0};
			if (auto it = detailsJSON.find("oX"); it != detailsJSON.end()) {
				origin.x = it.value();
			}
			if (auto it = detailsJSON.find("oY"); it != detailsJSON.end()) {
				origin.y = it.value();
			}

			// Load texture rect
			sf::IntRect rect {0, 0, 0, 0};
			if (auto it = detailsJSON.find("lX"); it != detailsJSON.end()) {
				rect.width = it.value();
			} else {
				rect.width = texture.getSize().x;
			}
			if (auto it = detailsJSON.find("lY"); it != detailsJSON.end()) {
				rect.height = it.value();
			} else {
				// For default, assume there are equal state frames spanning height...
				rect.top =  texture.getSize().y / statesJSON.size();
			}
			if (auto it = detailsJSON.find("pX"); it != detailsJSON.end()) {
				rect.left = it.value();
			}
			if (auto it = detailsJSON.find("pY"); it != detailsJSON.end()) {
				rect.top = it.value();
			} else {
				// ...and use equal interval
				rect.top =  statesCount * (texture.getSize().y / statesJSON.size());
			}

			++statesCount;

			// Push back states details
			this->statesDetails.push_back({
				statesIt.key(), // State name
				origin, 
				this->calculateVertices(rect)
			});
		}
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Could not load sprite object type: `" + this->name + "`."));
	}
	
	LOG_WARNING("{*", this, "\"", this->name, "} ", "Loaded with ", this->statesDetails.size(), " states.");
}



/* Functions */
std::array<sf::Vertex, 4> SpriteObjectType::calculateVertices(const sf::IntRect& rect)
{
	float width  = static_cast<float>(rect.width);
    float height = static_cast<float>(rect.height);
	float left   = static_cast<float>(rect.left);
    float top    = static_cast<float>(rect.top);
    float right  = left + width;
    float bottom = top + height;

	std::array<sf::Vertex, 4> vertices;

	vertices[0].position  = sf::Vector2f(0,     0);
    vertices[1].position  = sf::Vector2f(0,     height);
    vertices[2].position  = sf::Vector2f(width, 0);
    vertices[3].position  = sf::Vector2f(width, height);

    vertices[0].texCoords = sf::Vector2f(left,  top);
    vertices[1].texCoords = sf::Vector2f(left,  bottom);
    vertices[2].texCoords = sf::Vector2f(right, top);
    vertices[3].texCoords = sf::Vector2f(right, bottom);

	return vertices;
}

}
