#pragma once

#include <SFML/Graphics.hpp>

namespace rat {

class Script;

class BattleProvider {
public:

// Constructors

	BattleProvider(sf::RenderTarget& canvas);

// Drawing

	///
	void drawCircle(const sf::Vector2f& pos, float radius);

	///
	void drawCircle(float x, float y, float radius);

// Script

	static void initScript(Script& script);

private:

	sf::RenderTarget& canvas;
};

}