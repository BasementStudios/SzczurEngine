#pragma once

#include <cstdio>
#include <ctime>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "CoreModules.h"

namespace rat {
	class Object {
	private:

		CoreModules& _core;
		sf::Vector2f _pos;
		sf::Color _color;

	public:

		Object(CoreModules& core) :
			_core(core) {
			_pos.x = rand() % static_cast<int>(_core.get<Canvas>().getWindowSize().x);
			_pos.y = rand() % static_cast<int>(_core.get<Canvas>().getWindowSize().y);
			float r = std::abs(std::sin(_pos.x / 300.0f)) * 255.0f;
			float g = std::abs(std::sin(_pos.y / 300.0f)) * 255.0f;
			float b = std::abs(std::sin((_pos.x + _pos.y) / 300.0f)) * 255.f;
			_color = sf::Color(r, g, b);
		}

		void update(float deltaTime) {
			(void)deltaTime;
		}

		void render() {
			sf::CircleShape shape(30.f);
			shape.setFillColor(_color);
			shape.setOrigin({ 30.0f, 30.0f });
			shape.setPosition(_pos);

			_core.get<Canvas>().render(shape, Canvas::Layer::Ground);
		}
	};
}
