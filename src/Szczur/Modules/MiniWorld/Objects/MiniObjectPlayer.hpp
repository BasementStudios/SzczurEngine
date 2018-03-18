#pragma once

#include "Szczur/Modules/Input/Input.hpp"

#include "Szczur/Modules/MiniWorld/Objects/MiniObjectScene.hpp"

namespace rat {

struct MiniObjectPlayer : public MiniObjectScene {
	
	Input& input;
	
	float speed = 3;
	short int dir = 1;	

	MiniObjectPlayer(Script &script, Input& input);
	
/////////////////////////// METHODS ///////////////////////////

	
/////////////////////////// MAIN METHODS ///////////////////////////
	void update(float deltaTime);
	void editor();
	void render(sf::RenderTexture &canvas);
	
/////////////////////////// SCRIPT ///////////////////////////
	void runFileScript(const std::string& filepath);		
	void runScript(const std::string& code);
	static void initScript(Script& script);
};
	
}