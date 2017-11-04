#pragma once

#include <memory>

#include <SFML\Graphics\Sprite.hpp>


class WrapperSprite
{
public:
	std::unique_ptr<sf::Sprite>		sprite;
	sf::Transform					matrix;

	sf::BlendMode					blendMode;

	bool							visible;

public:
	WrapperSprite()
	{
		visible = true;
		blendMode = sf::RenderStates::Default.blendMode;
	}

	~WrapperSprite() = default;

	void render(sf::RenderTarget& target)
	{
		sf::RenderStates states;
		states.transform = matrix;
		states.blendMode = blendMode;

		if (visible)
			target.draw(*sprite, states);
	}
};

