#include "MiniObjectPlayer.hpp"

namespace rat {

	MiniObjectPlayer::MiniObjectPlayer(Script &script, Input& input) 
		: MiniObject(script), input(input) {
		name = "Player";
		tex.loadFromFile("assets/characters/char_1.png");
		spr.setTexture(tex);
		
		collider.setFillColor({180,120,180,120});
		collider.setOutlineColor({200,140,200,200});
		collider.setOutlineThickness(-1);
		collider.setPosition(colliderPos);
		collider.setSize(colliderSize);
	}	
	
/////////////////////////// METHODS ///////////////////////////

	bool MiniObjectPlayer::isCollision(const sf::Vector2f& bpos, const sf::Vector2f& bSize, const sf::Vector2f& bOffset) {
		sf::Vector2f aPos = pos + colliderPos - colliderSize/2.f;
		// aPos.x -= colliderSize.x/2.f;
		// aPos.y -= colliderSize.y/2.f;
		sf::Vector2f aSize = colliderSize;		
		sf::Vector2f bPos(bpos-bOffset);
		
		return (aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x &&
				aPos.y < bPos.y + bSize.y && aSize.y + aPos.y > bPos.y);
	}

/////////////////////////// MAIN METHODS ///////////////////////////
	
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
	}
	
	void MiniObjectPlayer::renderCollider(sf::RenderTexture& canvas) {
		collider.setOrigin(colliderSize.x/2.f, colliderSize.y/2.f);
		collider.setPosition(pos + colliderPos);
		collider.setSize(colliderSize);
		canvas.draw(collider);
	}
	
	void MiniObjectPlayer::render(sf::RenderTexture &canvas) {
		spr.setPosition(pos);
		spr.setOrigin(spr.getLocalBounds().width/2.f, spr.getLocalBounds().height/2.f);
		spr.setScale({0.2f * dir, 0.2f});
		canvas.draw(spr);
	}
	
	void MiniObjectPlayer::editor() {
		ImGui::Text("Player properties");
		ImGui::Separator();
		
// ================== Base ==================

		ImGui::Text("Base");
		ImGui::PushItemWidth(ImGui::GetWindowSize().x*0.3);
		ImGui::DragFloat("##PosX", &pos.x);
		ImGui::SameLine();
		ImGui::DragFloat("Pos##Y", &pos.y);
		ImGui::PopItemWidth();
		ImGui::Separator();
		
		ImGui::Text("Collider");
		ImGui::PushItemWidth(ImGui::GetWindowSize().x*0.3);
		ImGui::DragFloat("##PosColliderX", &colliderPos.x);
		ImGui::SameLine();
		ImGui::DragFloat("Pos##ColliderY", &colliderPos.y);
		ImGui::DragFloat("##SizeColliderX", &colliderSize.x);
		ImGui::SameLine();
		ImGui::DragFloat("Size##ColliderY", &colliderSize.y);
		ImGui::PopItemWidth();
		ImGui::Separator();
	}
}