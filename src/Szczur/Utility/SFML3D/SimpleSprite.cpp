#include "SimpleSprite.hpp"

void sf3d::SimpleSprite::setTexture(Texture * texture) {
	if(texture) {
		_texture = texture;
		_vertices.set(0u, {
			{-1.f, -1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{0.f, 0.f}
		});
		_vertices.set(1u, {
			{1.f, -1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{1.f, 0.f}
		});
		_vertices.set(2u, {
			{1.f, 1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{1.f, 1.f}
		});
		_vertices.set(3u, {
			{-1.f, 1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{0.f, 1.f}
		});
	}
}

void sf3d::SimpleSprite::draw(RenderTarget & target, RenderStates states) const {
	states.texture = _texture;
	target.simpleDraw(_vertices, states);
}
