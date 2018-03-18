#include "MiniObjectScene.hpp"

namespace rat {

	MiniObjectScene::MiniObjectScene(Script &script) 
		: MiniObject(script) {
		
		collider.setFillColor({180,120,180,120});
		collider.setOutlineColor({200,140,200,200});
		collider.setOutlineThickness(-1);
		collider.setPosition(colliderPos);
		collider.setSize(colliderSize);
	}	
	
/////////////////////////// METHODS ///////////////////////////
	bool MiniObjectScene::isCollision(const sf::Vector2f& bpos, const sf::Vector2f& bSize, const sf::Vector2f& bOffset) {
		sf::Vector2f aPos = pos + colliderPos - colliderSize/2.f;
		// aPos.x -= colliderSize.x/2.f;
		// aPos.y -= colliderSize.y/2.f;
		sf::Vector2f aSize = colliderSize;		
		sf::Vector2f bPos(bpos-bOffset);
		
		return (aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x &&
				aPos.y < bPos.y + bSize.y && aSize.y + aPos.y > bPos.y);
	}
	
/////////////////////////// MAIN METHODS ///////////////////////////
	
	void MiniObjectScene::loadTexture(const std::string& path) {
		texturePath = path;
		texture.loadFromFile(path);
		sprite.setTexture(texture, true);
		// frame.size.x = sprite.getLocalBounds().width*scale.x;
		// frame.size.y = sprite.getLocalBounds().height*scale.y;
	}
	
	void MiniObjectScene::setScale(float x, float y) {
		scale = sf::Vector2f(x,y);
		sprite.setScale(scale);
		// frame.size.x = sprite.getLocalBounds().width*scale.x*0.4f;
		// frame.size.y = sprite.getLocalBounds().height*scale.y*0.4f;
	}
	
	void MiniObjectScene::update(float deltaTime) {
	}
	
	void MiniObjectScene::renderCollider(sf::RenderTexture& canvas) {
		collider.setOrigin(colliderSize.x/2.f, colliderSize.y/2.f);
		collider.setPosition(pos + colliderPos);
		collider.setSize(colliderSize);
		canvas.draw(collider);
	}
	
	void MiniObjectScene::render(sf::RenderTexture &canvas) {
		// sprite.setTexture(texture);
		sprite.setPosition(pos);
		sprite.setOrigin(sprite.getLocalBounds().width/2.f, sprite.getLocalBounds().height/2.f);
		canvas.draw(sprite);
	}
	
	void MiniObjectScene::editor() {
		ImGui::Text("Scene object properties");
		
// ================== Name ==================

		static bool changeName = false;
		static char nameBuffer[50];
		if(!changeName) {
			if(ImGui::Button("Edit##Name", {40, 0})) {
				changeName = true;
				strcpy(nameBuffer, name.c_str());
				ImGui::SetKeyboardFocusHere(0);
			}
			ImGui::SameLine();
			ImGui::Text(name.c_str());
		}
		else {
			if(ImGui::InputText("New name##Input", nameBuffer, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
				setName(nameBuffer);
				changeName = false;
			}
		}		
		ImGui::Separator();
		
// ================== Base ==================

		ImGui::Text("Base");
		ImGui::PushItemWidth(ImGui::GetWindowSize().x*0.3);
		ImGui::DragFloat("##PosX", &pos.x);
		ImGui::SameLine();
		ImGui::DragFloat("Pos##Y", &pos.y);
		ImGui::PopItemWidth();
		ImGui::Separator();
		
// ================== Texture ==================

		static bool changeTexturePath = false;
		static char texturePathBuffer[100] = "assets/props/prop_1.png";
		ImGui::Text("Texture");		
		
		if(!changeTexturePath) {
			if(ImGui::Button("Edit##TexturePath", {40, 0})) {
				changeTexturePath = true;
				strcpy(texturePathBuffer, texturePath.c_str());
				ImGui::SetKeyboardFocusHere(0);
			}
			ImGui::SameLine();
			ImGui::Text(texturePath.c_str());
		}
		else {
			if(ImGui::InputText("New path##Input", texturePathBuffer, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
				loadTexture(texturePathBuffer);
				changeTexturePath = false;
			}
			if(ImGui::Button("Load texture", {100, 0})) {
				loadTexture(texturePathBuffer);
				changeTexturePath = false;
			}
		}
		
		ImGui::PushItemWidth(ImGui::GetWindowSize().x*0.3);
		if(ImGui::DragFloat("##ScaleX", &scale.x, 0.001, 0.001, 10)) {
			setScale(scale.x, scale.y);
		}
		ImGui::SameLine();
		if(ImGui::DragFloat("Scale##Y", &scale.y, 0.001, 0.001, 10)) {
			setScale(scale.x, scale.y);
		}
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