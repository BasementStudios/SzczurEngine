#pragma once

#include "Szczur/Modules/Input/Input.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

struct MiniObjectBackground : public MiniObject {
		
	std::string texturePath = "";
	sf::Texture texture;
	sf::Sprite sprite;
	
	float dis = 0;
	sf::Vector2f scale{1,1};
	
	void loadTexture(const std::string& path);
	void setScale(float x, float y);
	
	MiniObjectBackground(Script &script);
	
/////////////////////////// *ETTERS ///////////////////////////
	
/////////////////////////// MAIN METHODS ///////////////////////////
	void update();
	void editor();
	void render(sf::RenderTexture &canvas);		
};
	
}