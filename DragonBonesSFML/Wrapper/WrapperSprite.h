#pragma once

#include <memory>

#include <SFML\Graphics\Sprite.hpp>

class WrapperSprite
{
public:
	std::unique_ptr<sf::Sprite>		sprite;

public:
	WrapperSprite()
	{
		sprite = nullptr;
	}

	~WrapperSprite() = default;

	void render(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (sprite)
			target.draw(*sprite, states);
	}
};

