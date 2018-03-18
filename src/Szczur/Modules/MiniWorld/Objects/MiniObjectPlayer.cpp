#include "MiniObjectPlayer.hpp"

namespace rat {

	MiniObjectPlayer::MiniObjectPlayer(Script &script, Input& input) 
		: MiniObjectScene(script), input(input) {
		name = "Player";
		loadTexture("assets/player.png");
		setScale(0.2,0.2);
	}	
	
/////////////////////////// METHODS ///////////////////////////

/////////////////////////// MAIN METHODS ///////////////////////////
		
	void MiniObjectPlayer::render(sf::RenderTexture &canvas) {
		// sprite.setTexture(texture);
		sprite.setPosition(pos);
		sprite.setOrigin(sprite.getLocalBounds().width/2.f, sprite.getLocalBounds().height/2.f);
		sprite.setScale(scale.x*dir, scale.y);
		canvas.draw(sprite);
	}
	
	void MiniObjectPlayer::update(float deltaTime) {
		if(input.getManager().isKept(Keyboard::Left)) {
			pos.x -= speed*deltaTime*60.f;
			dir = -1;
		}
		if(input.getManager().isKept(Keyboard::Right)) {
			pos.x += speed*deltaTime*60.f;
			dir = 1;
		}
		if(input.getManager().isKept(Keyboard::Up)) {
			pos.y -= speed*deltaTime*60.f;
		}
		if(input.getManager().isKept(Keyboard::Down)) {
			pos.y += speed*deltaTime*60.f;
		}
		if(funcUpdate.valid()) funcUpdate(this);
	}
	
	void MiniObjectPlayer::editor() {
		MiniObjectScene::editor();
		
// ================== Player ==================

		ImGui::Text("Player");
		ImGui::PushItemWidth(ImGui::GetWindowSize().x*0.7);
		ImGui::SliderFloat("Speed##Player", &speed, 0, 12);
		ImGui::PopItemWidth();
		ImGui::Separator();
	}
	
/////////////////////////// SCRIPT ///////////////////////////
}