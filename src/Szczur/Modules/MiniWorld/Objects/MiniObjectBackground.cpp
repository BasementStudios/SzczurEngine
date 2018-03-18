#include "MiniObjectBackground.hpp"

namespace rat {

	MiniObjectBackground::MiniObjectBackground(Script &script) 
		: MiniObject(script) {
		// loadTexture("assets/characters/char_1.png"
		// texture.loadFromFile("assets/characters/char_1.png");
		// spr.setTexture(tex);
	}	
	
/////////////////////////// MAIN METHODS ///////////////////////////
	
	void MiniObjectBackground::loadTexture(const std::string& path) {
		texturePath = path;
		texture.loadFromFile(path);
		sprite.setTexture(texture, true);
		frame.size.x = sprite.getLocalBounds().width*scale.x;
		frame.size.y = sprite.getLocalBounds().height*scale.y;
	}
	
	void MiniObjectBackground::setScale(float x, float y) {
		scale = sf::Vector2f(x,y);
		sprite.setScale(scale);
		frame.size.x = sprite.getLocalBounds().width*scale.x;
		frame.size.y = sprite.getLocalBounds().height*scale.y;
	}
	
	void MiniObjectBackground::update() {
	}
	
	void MiniObjectBackground::editor() {
		ImGui::Text("Scene object properties");
		
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
			if(ImGui::InputText("New name##Input", texturePathBuffer, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
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
	}
	
	void MiniObjectBackground::render(sf::RenderTexture &canvas) {
		// sprite.setTexture(texture);
		sprite.setPosition(pos);
		sprite.setOrigin(sprite.getLocalBounds().width/2.f, sprite.getLocalBounds().height/2.f);
		canvas.draw(sprite);
	}
}