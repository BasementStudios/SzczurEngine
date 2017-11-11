#pragma once

#include <memory>

#include "Mesh.h"

class WrapperDisplay
{
public:
	std::unique_ptr<Mesh>			_meshDisplay;
	std::unique_ptr<sf::Sprite>		_spriteDisplay;

	sf::Transform					matrix;
	sf::BlendMode					blendMode;
	bool							visible;

public:
	WrapperDisplay()
	{
		_meshDisplay = nullptr;
		_spriteDisplay = nullptr;

		visible = true;
	}

	~WrapperDisplay() = default;

	void render(sf::RenderTarget &window, sf::RenderStates states)
	{
		if (visible)
		{
			states.blendMode = blendMode;
			states.transform *= matrix;

			if (_spriteDisplay)
				window.draw(*_spriteDisplay, states);

			if (_meshDisplay)
				_meshDisplay->render(window, states);
		}
	}
};

