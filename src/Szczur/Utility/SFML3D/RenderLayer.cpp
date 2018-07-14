#include "RenderLayer.hpp"

/** @file RenderLayer.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec2.hpp>

#include "Drawable.hpp"
#include "RenderTexture.hpp"
#include "SimpleSprite.hpp"
#include "ShaderProgram.hpp"

namespace sf3d
{

/* Operators */
RenderLayer::RenderLayer()
{
	;
}
RenderLayer::~RenderLayer()
{
	;
}

RenderLayer::RenderLayer(glm::uvec2 size, ShaderProgram* program)
{
	this->create(size, program);
}



/* Methods */
void RenderLayer::create(glm::uvec2 size, ShaderProgram* program)
{
	RenderTexture::create(size, program);
	
	this->sprite.setTexture(&(this->getTexture()));
}

void RenderLayer::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(this->sprite, states);
}

}
