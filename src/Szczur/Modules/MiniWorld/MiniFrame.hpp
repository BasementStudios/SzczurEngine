#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Window/Window.hpp"

namespace rat {

struct MiniFrame {

	sf::Vector2f *pos = nullptr;
	sf::Vector2f size{20,20};
	sf::RectangleShape shape;
	bool clickable = true;
	
	MiniFrame(sf::Vector2f& targetPos);
	bool isHovered(const sf::Vector2f& point);
	void setClickable(bool flag);
	bool getClickable();
	const sf::RectangleShape& getShape(int state);
};

}