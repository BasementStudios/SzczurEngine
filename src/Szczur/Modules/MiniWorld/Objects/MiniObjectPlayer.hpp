#pragma once

#include "Szczur/Modules/Input/Input.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

struct MiniObjectPlayer : public MiniObject {
	
	Input& input;
	sf::Texture tex;
	sf::Sprite spr;
	
	int speed = 3;
	short int dir = 0;
	
	MiniObjectPlayer(Script &script, Input& input);
	
/////////////////////////// *ETTERS ///////////////////////////
	
/////////////////////////// MAIN METHODS ///////////////////////////
	void update();
	void editor();
	void render(sf::RenderTexture &canvas);		
};
	
}