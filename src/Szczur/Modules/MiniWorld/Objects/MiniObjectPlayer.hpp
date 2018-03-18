#pragma once

#include "Szczur/Modules/Input/Input.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

struct MiniObjectPlayer : public MiniObject {
	
	Input& input;
	sf::Texture tex;
	sf::Sprite spr;
	
	float speed = 3;
	short int dir = 1;	
	
	sf::RectangleShape collider;
	sf::Vector2f colliderPos{0,0};
	sf::Vector2f colliderSize{100,100};
	
	MiniObjectPlayer(Script &script, Input& input);
	
/////////////////////////// METHODS ///////////////////////////
	bool isCollision(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& offset);
	
/////////////////////////// MAIN METHODS ///////////////////////////
	void update(float deltaTime);
	void editor();
	void render(sf::RenderTexture &canvas);		
	void renderCollider(sf::RenderTexture& canvas);
};
	
}