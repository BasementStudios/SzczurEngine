#pragma once

#include "Szczur/Modules/Input/Input.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

struct MiniObjectScene : public MiniObject {
		
	std::string texturePath = "";
	sf::Texture texture;
	sf::Sprite sprite;
	
	float dis = 0;
	sf::Vector2f scale{1,1};
	
	sf::RectangleShape collider;
	sf::Vector2f colliderPos{0,0};
	sf::Vector2f colliderSize{100,100};
	
	sol::function funcOnAction;
	sol::function funcUpdate;
	
	void loadTexture(const std::string& path);
	void setScale(float x, float y);
	
	MiniObjectScene(Script &script);
	
/////////////////////////// METHODS ///////////////////////////
	bool isCollision(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& offset);
	void action();
	void renderCollider(sf::RenderTexture& canvas);
	
/////////////////////////// MAIN METHODS ///////////////////////////
	virtual void update(float deltaTime);
	virtual void editor();
	virtual void render(sf::RenderTexture &canvas);
	
/////////////////////////// SCRIPT ///////////////////////////
	void runFileScript(const std::string& filepath);		
	void runScript(const std::string& code);
};
	
}