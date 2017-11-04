#pragma once

#include <memory>

#include <SFML\Graphics\Sprite.hpp>


class WrapperSprite
{
public:
	std::unique_ptr<sf::Sprite>		sprite;
	sf::Transform					matrix;

public:
	WrapperSprite() = default;
	~WrapperSprite() = default;

	void render(sf::RenderTarget& target)
	{
		target.draw(*sprite, matrix);
	}
};
