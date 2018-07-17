#include "Sprite.hpp"

#include <glm/vec2.hpp>

#include "RenderTarget.hpp"
#include "Texture.hpp"

namespace sf3d
{

/* Properties */
void Sprite::setTexture(const Texture& texture)
{
	_texture = &texture;

	const glm::vec2 size = texture.getSize();
	_vertices[0] = {
		{0.f, 0.f, 0.f},
		{1.f, 1.f, 1.f, 1.f},
		{0.f, 0.f}
	};
	_vertices[1] = {
		{size.x, 0.f, 0.f},
		{1.f, 1.f, 1.f, 1.f},
		{1.f, 0.f}
	};
	_vertices[2] = {
		{size.x, -size.y, 0.f},
		{1.f, 1.f, 1.f, 1.f},
		{1.f, 1.f}
	};
	_vertices[3] = {
		{0.f, -size.y, 0.f},
		{1.f, 1.f, 1.f, 1.f},
		{0.f, 1.f}
	};
}



/* Operators */
Sprite::Sprite()
{
	;
}

Sprite::Sprite(const Texture& texture) 
{
	setTexture(texture);
}



/* Methods */
void Sprite::draw(RenderTarget & target, RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = _texture;
	target.draw(_vertices, states);
}

}
