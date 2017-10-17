#pragma once

#include <cstdio>
#include <ctime>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Core.h"

namespace rat {
	class Object {
	private:
		Core& core;
		sf::Vector2f pos;
		sf::Color color;
	
	public:
		Object(Core& _core) :
			core(_core) {
			pos.x = rand()%static_cast<int>(core.canvas.getWindowSize().x);
			pos.y = rand()%static_cast<int>(core.canvas.getWindowSize().y);
			float r = std::fabs(std::sin(pos.x/300.f))*255.f;
			float g = std::fabs(std::sin(pos.y/300.f))*255.f;
			float b = std::fabs(std::sin((pos.x+pos.y)/300.f))*255.f;
			color = sf::Color(r,g,b);
		}
		void update(float deltaTime) {
			
		}
		void render() {
			sf::CircleShape shape(30.f);
			shape.setFillColor(color);
			shape.setOrigin(sf::Vector2f(30.f, 30.f));
			shape.setPosition(pos);
			
			core.canvas.render(shape, 0);
			// Alternative:
			// Core::getStaticCanvas().render(shape, 0);
		}
	};
}