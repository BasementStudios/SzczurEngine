#include "Sprite.hpp"

#include "RenderTarget.hpp"
#include "Texture.hpp"

namespace sf3d {
	Sprite::Sprite() {

	}

	void Sprite::setTexture(Texture* texture) {
		if(texture) {
			_texture = texture;
			auto size = texture->getSize();
			_vertices[0] = {
				{0.f, 0.f, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{0.f, 0.f}
			};
			_vertices[1] = {
				{(float)size.x, 0.f, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{1.f, 0.f}
			};
			_vertices[2] = {
				{(float)size.x, -(float)size.y, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{1.f, 1.f}
			};
			_vertices[3] = {
				{0.f, -(float)size.y, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{0.f, 1.f}
			};
		}
	}

    void Sprite::setTextureRect(glm::vec2 position, glm::vec2 size) {
        if(_texture) {
            glm::vec2 texSize = _texture->getSize();
            if(texSize.x > 0 && texSize.y > 0) {
                _vertices[0] = {
                    {0.f, 0.f, 0.f},
                    {1.f, 1.f, 1.f, 1.f},
                    {position.x / texSize.x, position.y / texSize.y}
                };
                _vertices[1] = {
                    {size.x, 0.f, 0.f},
                    {1.f, 1.f, 1.f, 1.f},
                    {(position.x + size.x) / texSize.x, position.y / texSize.y}
                    //{1.f, 0.f}
                };
                _vertices[2] = {
                    {size.x, -size.y, 0.f},
                    {1.f, 1.f, 1.f, 1.f},
                    {(position.x + size.x) / texSize.x, (position.y + size.y) / texSize.y}
                    //{1.f, 1.f}
                };
                _vertices[3] = {
                    {0.f, -size.y, 0.f},
                    {1.f, 1.f, 1.f, 1.f},
                    {position.x / texSize.x, (position.y + size.y) / texSize.y}
                    //{0.f, 1.f}
                };
            }
        }
    }

	void Sprite::draw(RenderTarget & target, RenderStates states) const {
		states.transform *= getTransform();
		states.texture = _texture;
		target.draw(_vertices, states);
	}
}
