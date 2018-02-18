#include "SpriteDisplayer.hpp"

/** @file SpriteDisplayer.hpp
 ** @description Implementaion file for SpriteDisplayer class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "Szczur/Modules/Assets/Asset.hpp"
#include "Szczur/Modules/World/SceneObject.hpp"

namespace rat
{

/* Properties */
void SpriteDisplayer<SceneObject>::setTextureRect(const sf::IntRect rect)
{
	if(this->_textureRect != rect) {
		this->_textureRect = rect;
		this->updateVertices();
	}
}



/* Operators */
SpriteDisplayer<SceneObject>::SpriteDisplayer(Assets& assets, SceneObject& object)
{
	// @todo . loading v info o obj
	
	// @todo ! ratAsset do użycia
	// @info For testing purposes World_Some1.png
	//_textureAsset = &assets.load<sf::Asset<sf::Texture>>("Assets/Objects/" + object.getType() + "/" + object.getState() + ".png");
	this->_texture = &(assets.load<sf::Texture>("Assets/Objects/" + object.getType() + "/" + object.getState() + ".png"));

	// @todo ! ładowanie texrect z json
	//this->setTextureRect(sf::IntRect(0, 0, _textureAsset.getRef().getSize().x, _textureAsset.getRef().getSize().y));
	this->setTextureRect(sf::IntRect(0, 0, _texture->getSize().x, _texture->getSize().y));
}
SpriteDisplayer<SceneObject>::~SpriteDisplayer() 
{
	// @todo ! ratAsset do użycia
	//_textureAsset.unload();
}



/* Methods */
void SpriteDisplayer<SceneObject>::draw(const SceneObject& object, sf::RenderTarget& target, sf::RenderStates states) const
{
	// @todo ! nałożyć transformacje kamery
	states.transform
		.translate(object.getPosition())
		.translate(-(this->_origin))
	;
	
	// @todo ! ratAsset do użycia
	//states.texture = this->_textureAsset.getPtr();
	states.texture = this->_texture;

	target.draw(this->_vertices, 4, sf::PrimitiveType::TriangleStrip, states);
}

void SpriteDisplayer<SceneObject>::updateVertices()
{
	float width  = static_cast<float>(this->_textureRect.width);
    float height = static_cast<float>(this->_textureRect.height);
	float left   = static_cast<float>(this->_textureRect.left);
    float top    = static_cast<float>(this->_textureRect.top);
    float right  = left + width;
    float bottom = top + height;

	this->_vertices[0].position  = sf::Vector2f(0,     0);
    this->_vertices[1].position  = sf::Vector2f(0,     height);
    this->_vertices[2].position  = sf::Vector2f(width, 0);
    this->_vertices[3].position  = sf::Vector2f(width, height);

    this->_vertices[0].texCoords = sf::Vector2f(left,  top);
    this->_vertices[1].texCoords = sf::Vector2f(left,  bottom);
    this->_vertices[2].texCoords = sf::Vector2f(right, top);
    this->_vertices[3].texCoords = sf::Vector2f(right, bottom);
}

}
