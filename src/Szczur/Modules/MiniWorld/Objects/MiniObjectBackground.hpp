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
	void update(float deltaTime);
	void editor();
	void render(sf::RenderTexture &canvas);
	
	// Parallax effect
	void render(sf::RenderTexture &canvas, const sf::Vector2f& target);
	
/////////////////////////// SCRIPT ///////////////////////////
	void runFileScript(const std::string& filepath){}		
	void runScript(const std::string& code){}
	static void initScript(Script& script);
};
	
}