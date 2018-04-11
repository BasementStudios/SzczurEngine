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
SpriteComponent::SpriteComponent(SpriteDisplayData* displayData)
	: displayData(displayData)
{};
SpriteComponent::SpriteComponent(SpriteComponent&& other)
	: displayData(other.displayData)
{};



/* Methods */
/// draw
void SpriteComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	states.transform *= this->getTransform();
	states.texture = &(this->getTexture());
	target.draw(this->getVertices(), states);
}

}
