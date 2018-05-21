#pragma once

#include <SFML/Graphics.hpp>

namespace rat {

class BattleTexture {
public:

	///
	bool load(const std::string& filePath, int start_x, int start_y, int frame_w, int frame_h, int framesInRow);

	///
	bool good() const;

	///
	sf::Sprite getSprite(int frame) const;

private:

	bool isGood = false;
	std::string filePath;
	sf::Texture texture;
	int start_x;
	int start_y;
	int frame_w;
	int frame_h;
	int framesInRow;
};

}