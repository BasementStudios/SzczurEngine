#include "SimpleSprite.hpp"

void sf3d::SimpleSprite::setTexture(Texture * texture) {
	if(texture) {
		_texture = texture;
		_vertices[0] = {
			{-1.f, -1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{0.f, 0.f}
		};
		_vertices[1] = {
			{1.f, -1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{1.f, 0.f}
		};
		_vertices[2] = {
			{1.f, 1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{1.f, 1.f}
		};
		_vertices[3] = {
			{-1.f, 1.f, 0.f},
			{1.f, 1.f, 1.f, 1.f},
			{0.f, 1.f}
		};
	}
}

void sf3d::SimpleSprite::draw(RenderTarget & target, RenderStates states) const {
	states.texture = _texture;
	target.simpleDraw(_vertices, states);
}
