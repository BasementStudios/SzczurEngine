#include "MiniObjectPlayer.hpp"

namespace rat {

	MiniObjectPlayer::MiniObjectPlayer(Script &script, Input& input) 
		: MiniObject(script), input(input) {
		name = "Player";
		tex.loadFromFile("assets/characters/char_1.png");
		spr.setTexture(tex);
	}	
	
/////////////////////////// MAIN METHODS ///////////////////////////
	
	void MiniObjectPlayer::update() {
		if(input.getManager().isKept(Keyboard::Left)) {
			pos.x -= speed;
			dir = -1;
		}
		if(input.getManager().isKept(Keyboard::Right)) {
			pos.x += speed;
			dir = 1;
		}
		if(input.getManager().isKept(Keyboard::Up)) {
			pos.y -= speed;
		}
		if(input.getManager().isKept(Keyboard::Down)) {
			pos.y += speed;
		}
	}
	
	void MiniObjectPlayer::render(sf::RenderTexture &canvas) {
		spr.setPosition(pos);
		spr.setOrigin(spr.getLocalBounds().width/2.f, spr.getLocalBounds().height/2.f);
		spr.setScale({0.2f * dir, 0.2f});
		canvas.draw(spr);
	}
	
	void MiniObjectPlayer::editor() {
		ImGui::Text("Player properties");
		ImGui::Text("%s", name.c_str());
	}
}