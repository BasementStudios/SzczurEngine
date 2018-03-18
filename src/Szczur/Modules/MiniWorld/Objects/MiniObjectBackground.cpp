#include "MiniObjectBackground.hpp"

#include "Szczur/Modules/FileSystem/FileDialog.hpp"

namespace rat {

	MiniObjectBackground::MiniObjectBackground(Script &script) 
		: MiniObject(script) {
		type = Types::Background;
		// loadTexture("assets/characters/char_1.png"
		// texture.loadFromFile("assets/characters/char_1.png");
		// spr.setTexture(tex);
	}	
	
/////////////////////////// MAIN METHODS ///////////////////////////
	
	void MiniObjectBackground::loadTexture(const std::string& path) {
		texturePath = path;
		texture.loadFromFile(path);
		sprite.setTexture(texture, true);
		frame.size.x = sprite.getLocalBounds().width*scale.x*0.4f;
		frame.size.y = sprite.getLocalBounds().height*scale.y*0.4f;
	}
	
	void MiniObjectBackground::setScale(float x, float y) {
		scale = sf::Vector2f(x,y);
		sprite.setScale(scale);
		frame.size.x = sprite.getLocalBounds().width*scale.x*0.4f;
		frame.size.y = sprite.getLocalBounds().height*scale.y*0.4f;
	}
	
	void MiniObjectBackground::update(float deltaTime) {
	}
	
	void MiniObjectBackground::editor() {
		ImGui::Text("Background object properties");
		
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
			if(ImGui::InputText("New path##Input", nameBuffer, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
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
			if(ImGui::InputText("New name##Input", texturePathBuffer, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
				loadTexture(texturePathBuffer);
				changeTexturePath = false;
			}
			if(ImGui::Button("Load texture...", {120, 0})) {
				
				std::string foundedPath = FileDialog::getOpenFileName("Texture", ".");
				loadTexture(foundedPath);
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
		
	// Parallax effect
	
		ImGui::Text("Parallax effect");		
		ImGui::PushItemWidth(ImGui::GetWindowSize().x*0.3);
		ImGui::DragFloat("Distance", &dis, 0.1, -100, 100);
		ImGui::PopItemWidth();
		ImGui::Separator();
	}
	
	void MiniObjectBackground::render(sf::RenderTexture &canvas) {
		// sprite.setTexture(texture);
		sprite.setPosition(pos);
		sprite.setOrigin(sprite.getLocalBounds().width/2.f, sprite.getLocalBounds().height/2.f);
		canvas.draw(sprite);
	}
	
	void MiniObjectBackground::render(sf::RenderTexture &canvas, const sf::Vector2f& target) {		
		sprite.setPosition(pos);
		sprite.setOrigin(sprite.getLocalBounds().width/2.f, sprite.getLocalBounds().height/2.f);		
		sf::Vector2f delta = (pos - target)*dis/100.f;
		sprite.move(delta.x, 0);
		
		canvas.draw(sprite);
	}

	
/////////////////////////// SCRIPT ///////////////////////////
	
	void MiniObjectBackground::initScript(Script& script) {
		auto object = script.newClass<MiniObjectBackground>("MiniObjectBackground", "MiniWorld");
		
		// Base
		object.set("pos", &MiniObject::pos);
		object.set("getName", &MiniObject::getName);
		object.set("setName", &MiniObject::setName);
		
		// Background
		object.set("loadTexture", &MiniObjectBackground::loadTexture);
		object.set("setScale", &MiniObjectBackground::setScale);
		object.set("runFileScript", &MiniObjectBackground::runFileScript);
		object.set("runScript", &MiniObjectBackground::runScript);
		
		object.init();
	}
}