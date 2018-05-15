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
			_vertices.set(0u, {
				{0.f, 0.f, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{0.f, 0.f}
			});
			_vertices.set(1u, {
				{(float)size.x, 0.f, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{1.f, 0.f}
			});
			_vertices.set(2u, {
				{(float)size.x, -(float)size.y, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{1.f, 1.f}
			});
			_vertices.set(3u, {
				{0.f, -(float)size.y, 0.f},
				{1.f, 1.f, 1.f, 1.f},
				{0.f, 1.f}
			});
		}
	}

	void Sprite::draw(RenderTarget & target, RenderStates states) const {
		states.transform *= getTransform();
		states.texture = _texture;
		target.draw(_vertices, states);
	}
}
