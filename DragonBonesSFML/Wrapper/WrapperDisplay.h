#pragma once

#include <memory>

#include "WrapperMesh.h"
#include "WrapperSprite.h"

class WrapperDisplay
{
public:
	WrapperMesh						*_meshDisplay;
	WrapperSprite					*_spriteDisplay;

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

	~WrapperDisplay()
	{
		if (_meshDisplay)
		{
			delete _meshDisplay;
			_meshDisplay = nullptr;
		}

		if (_spriteDisplay)
		{
			delete _spriteDisplay;
			_spriteDisplay = nullptr;
		}
	}

	void render(sf::RenderTarget &window)
	{
		if (visible)
		{
			sf::RenderStates states;
			states.blendMode = blendMode;
			states.transform = matrix;

			if (_spriteDisplay)
				_spriteDisplay->render(window, states);

			if (_meshDisplay)
				_meshDisplay->render(window, states);
		}
	}
};

