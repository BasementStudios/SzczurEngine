#include "SpriteComponent.hpp"

/** @file SpriteComponent.cpp
 ** @description Implementation file for sprite component class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Modules/World/DisplayData/SpriteDisplayData.hpp"

namespace rat
{

/* Properties */
/// Texture
const sf3d::Texture& SpriteComponent::getTexture() const
{
	return this->displayData->getTexture();
}

/// Veritces
const sf3d::VertexArray& SpriteComponent::getVertices() const
{
	return this->displayData->getVertices();
}



/* Operators */
SpriteComponent::SpriteComponent()
{};
SpriteComponent::SpriteComponent(SpriteDisplayData* displayData) {
	setDisplayData(displayData);
};
SpriteComponent::SpriteComponent(SpriteComponent&& other) {
	setDisplayData(other.displayData);
};

void SpriteComponent::setDisplayData(SpriteDisplayData* displayData) {
	this->displayData = displayData;
	if(displayData) {
		const auto& size = displayData->getTexture().getSize();
		setOrigin({(float)size.x/2.f, (float)size.y, 0.f});
	}
}

SpriteDisplayData* SpriteComponent::getDisplayData() const {
	return displayData;
}


/* Methods */
/// draw
void SpriteComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	if(displayData) {
		states.transform *= this->getTransform();
		states.texture = &(this->getTexture());
		target.draw(this->getVertices(), states);
	}
}

}
