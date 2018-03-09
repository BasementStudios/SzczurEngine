#include "SpriteObjectType.hpp"

/** @file SpriteObjectType.cpp
 ** @description Implementaion file with sprite object type class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <array>
#include <vector>
#include <fstream>
#include <exception>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Szczur/JSON.hpp"
#include "Szczur/Debug/Logger.hpp"
#include "Object.hpp"
#include "ObjectType.hpp"

namespace rat
{

/* Properties */
/// Texture
const sf::Texture& SpriteObjectType::getTexture() const
{
	return this->texture;
}

/// Origin
const Object::Vector_t SpriteObjectType::getOrigin(Object::StateID_t stateID) const
{
	if (this->statesDetails.size() < stateID) {
		return this->statesDetails[stateID].origin;
	}
	// Fallback
	LOG_WARN("[World][ObjectType{*", this, ";\"", this->name, "}] ",
		"Origin for Sprite display for ID `", stateID, "` was not found; using default.")
	return this->statesDetails[ObjectType::defaultStateID].origin;
}

/// Vertices
const std::array<sf::Vertex, 4> SpriteObjectType::getVertices(Object::StateID_t stateID) const
{
	if (this->statesDetails.size() < stateID) {
		return this->statesDetails[stateID].vertices;
	}
	// Fallback
	LOG_WARN("[World][ObjectType{*", this, ";\"", this->name, "}] ",
		"Vertices for Sprite display for ID `", stateID, "` was not found; using default.")
	return this->statesDetails[ObjectType::defaultStateID].vertices;
}



/* Operators */
/// Constructor 
SpriteObjectType::SpriteObjectType(const std::string& name)
{
	// Load texture file
	if (!texture.loadFromFile(SpriteObjectType::assetsPath + name + '/' + SpriteObjectType::textureFileName)) {
		LOG_ERROR("[World][ObjectType{*", this, ";\"", this->name, "}] ",
			"Cannot load texture file for this Sprite object type!");
		return; // @todo exception?
	}
	
	// Open config file
	std::ifstream file(SpriteObjectType::assetsPath + name + '/' + SpriteObjectType::configFileName);
	if (!file.is_open()) {
		LOG_ERROR("[World][ObjectType{*", this, ";\"", this->name, "}] ",
			"Cannot load config file for this Sprite object type!");
		return; // @todo exception?
	}
	
	try {
		// Load JSON 
		json configJSON;
		file >> configJSON;

		auto& statesJSON = configJSON["states"];
		this->statesDetails.reserve(statesJSON.size());

		std::size_t statesCount = 0;

		for (json::iterator statesIt = statesJSON.begin(); statesIt != statesJSON.end(); ++statesIt) {
			// Load state name
			this->statesStrings.push_back(statesIt.key());

			auto& detailsJSON = statesIt.value();
			
			// Load origin
			Object::Vector_t origin {0, 0};
			if (auto it = detailsJSON.find("oX"); it != detailsJSON.end()) {
				origin.x = it.value();
			}
			if (auto it = detailsJSON.find("oY"); it != detailsJSON.end()) {
				origin.x = it.value();
			}

			// Load texture rect
			sf::IntRect rect {0, 0, 0, 0};
			if (auto it = detailsJSON.find("lX"); it != detailsJSON.end()) {
				rect.width = it.value();
			}
			if (auto it = detailsJSON.find("lY"); it != detailsJSON.end()) {
				rect.height = it.value();
			}
			if (auto it = detailsJSON.find("pX"); it != detailsJSON.end()) {
				rect.left = it.value();
			}
			if (auto it = detailsJSON.find("pY"); it != detailsJSON.end()) {
				rect.top = it.value();
			} else {
				// For default, assume there are equal state frames spanning height.
				rect.top =  statesCount * (texture.getSize().y / statesJSON.size());
			}

			++statesCount;

			this->statesDetails.push_back({
				origin, 
				this->calculateVertices(rect)
			});
		}
	} 
	catch(std::exception& e) {
		LOG_ERROR("[World][ObjectType{*", this, ";\"", this->name, "}] ",
			"Error while parsing configuration file for this Sprite object type!\n",
			" Exception message: ", e.what());
		return; // @todo exception?
	}
	
	LOG_INFO("[World][ObjectType{*", this, ";\"", this->name, "}] ",
		"Sprite object type loaded with ", statesDetails.size(), " states.");
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
