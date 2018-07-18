#include "RenderLayer.hpp"

/** @file RenderLayer.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec2.hpp>

#include "Drawable.hpp"
#include "RenderTexture.hpp"
#include "VertexArray.hpp"
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

	// Setup vertices (inverted since framebuffer needs this)
	this->vertices[0].position = {-1.f, 1.f, 0.f};
	this->vertices[0].texCoord = {0.f, 1.f};

	this->vertices[1].position = { 1.f, 1.f, 0.f};
	this->vertices[1].texCoord = {1.f, 1.f};

	this->vertices[2].position = { 1.f, -1.f, 0.f};
	this->vertices[2].texCoord = {1.f, 0.f};

	this->vertices[3].position = {-1.f, -1.f, 0.f};
	this->vertices[3].texCoord = {0.f, 0.f};
}

void RenderLayer::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = &(this->getTexture());
	target.draw(this->vertices, states);
}

}
