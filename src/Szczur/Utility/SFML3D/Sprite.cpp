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

	_vertices[0].position = {0.f, 0.f, 0.f};
	_vertices[0].texCoord = {0.f, 0.f};

	_vertices[1].position = {size.x, 0.f, 0.f};
	_vertices[1].texCoord = {1.f, 0.f};

	_vertices[2].position = {size.x, -size.y, 0.f};
	_vertices[2].texCoord = {1.f, 1.f};
	
	_vertices[3].position = {0.f, -size.y, 0.f};
	_vertices[3].texCoord = {0.f, 1.f};
}

void Sprite::setTextureRect(const glm::uvec2& position, const glm::uvec2& size)
{
	if (_texture)
	{
		glm::vec2 texSize = _texture->getSize();
		if (texSize.x > 0 && texSize.y > 0)
		{
			_vertices[0] = {
				{ 0.f, 0.f, 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ position.x / texSize.x, position.y / texSize.y }
			};
			_vertices[1] = {
				{ size.x, 0.f, 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ (position.x + size.x) / texSize.x, position.y / texSize.y }
			//{1.f, 0.f}
			};
			_vertices[2] = {
				{ size.x, -static_cast<float>(size.y), 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ (position.x + size.x) / texSize.x, (position.y + size.y) / texSize.y }
			//{1.f, 1.f}
			};
			_vertices[3] = {
				{ 0.f, -static_cast<float>(size.y), 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ position.x / texSize.x, (position.y + size.y) / texSize.y }
			//{0.f, 1.f}
			};
		}
	}
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
