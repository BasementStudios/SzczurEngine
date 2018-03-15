#include "SpriteObjectType.hpp"

/** @file SpriteObjectType.cpp
 ** @description Implementaion with Sprite Object Type class as shared game sprite object type information.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <array>
#include <vector>
#include <fstream>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Szczur/JSON.hpp"
#include "Szczur/Debug.hpp"
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
const std::string& SpriteObjectType::getStateString(SpriteObjectType::StateID_t stateID) const
{
	if (this->statesDetails.size() > stateID) {
		return this->statesDetails[stateID].name;
	}
	LOG_WARN("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ",
		"State ID `", stateID, "` was not found; using default.");
	return this->statesDetails[SpriteObjectType::defaultStateID].name;
}
SpriteObjectType::StateID_t SpriteObjectType::getStateID(const std::string& stateString) const
{
    for (std::vector<int>::size_type i = 0, I = this->statesDetails.size(); i != I; i++) {
        if (this->statesDetails[i].name == stateString) {
            return i;
        }
    }
    LOG_WARN("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ",
		"State ID for string `", stateString, "` was not found; using default.");
	return SpriteObjectType::defaultStateID;
}

/// Origin
const Object::Vector_t SpriteObjectType::getOrigin(SpriteObjectType::StateID_t stateID) const
{
	if (this->statesDetails.size() < stateID) {
		return this->statesDetails[stateID].origin;
	}
	// Fallback
	LOG_WARN("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ",
		"Origin for state ID`", stateID, "` was not found; using default.")
	return this->statesDetails[SpriteObjectType::defaultStateID].origin;
}

/// Vertices
const std::array<sf::Vertex, 4> SpriteObjectType::getVertices(SpriteObjectType::StateID_t stateID) const
{
	if (this->statesDetails.size() < stateID) {
		return this->statesDetails[stateID].vertices;
	}
	// Fallback
	LOG_WARN("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ",
		"Vertices for state ID `", stateID, "` was not found; using default.")
	return this->statesDetails[SpriteObjectType::defaultStateID].vertices;
}



/* Operators */
/// Constructor 
SpriteObjectType::SpriteObjectType(const std::string& typeName)
{
	// Load texture file
	if (!this->texture.loadFromFile(SpriteObjectType::assetsPath + typeName + '/' + SpriteObjectType::textureFileName)) {
		LOG_ERROR("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ", 
			"Cannot load texture file!");
		return; // @todo exception?
	}
	
	// Open config file
	std::ifstream file(SpriteObjectType::assetsPath + typeName + '/' + SpriteObjectType::configFileName);
	if (!file.is_open()) {
		LOG_ERROR("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ", 
			"Cannot load config file!");
		return; // @todo exception?
	}
	
	try {
		// Load JSON 
		json configJSON;
		file >> configJSON;

		auto& statesJSON = configJSON["states"];
		this->statesDetails.reserve(statesJSON.size());

		// Counting states to dive texture to sprite frames
		std::size_t statesCount = 0;

		// Load states
		for (auto statesIt = statesJSON.begin(); statesIt != statesJSON.end(); ++statesIt) {
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

			// Push back states details
			this->statesDetails.push_back({
				statesIt.key(), // State name
				origin, 
				this->calculateVertices(rect)
			});
		}
	} 
	catch(std::exception& e) {
		LOG_ERROR("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ", 
			"Error while parsing configuration file! Exception message: ", e.what());
		return; // @todo exception?
	}
	
	LOG_INFO("[World][SpriteObjectType{*", this, ";\"", this->name, "}] ", 
		"Loaded with ", statesDetails.size(), " states.");
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
