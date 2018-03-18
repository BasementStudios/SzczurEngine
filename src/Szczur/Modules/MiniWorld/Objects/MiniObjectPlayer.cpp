#include "MiniObjectPlayer.hpp"

#include "Szczur/Modules/DragonBones/DragonBones.hpp"

namespace rat {

	MiniObjectPlayer::MiniObjectPlayer(Script &script, Input& input) 
		: MiniObjectScene(script), input(input) {
		type = Types::Player;
		
		name = "Player";
		// loadTexture("assets/textures/player.png");
		setScale(0.1,0.1);
		armature = modulePtr_v<DragonBones>->createArmature("Cedmin");
		armature->playAnimation("Cedmin_Run_051");
	}	
	
/////////////////////////// METHODS ///////////////////////////

/////////////////////////// MAIN METHODS ///////////////////////////
		
	void MiniObjectPlayer::playAnimation(int id) {
	
	// void fadeInAnimation(const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1, int layer = 0, 
		// const std::string& group = "", dragonBones::AnimationFadeOutMode fadeOutMode = dragonBones::AnimationFadeOutMode::SameLayerAndGroup) 
	
		if(id == 0 && id != currId) {
			armature->fadeInAnimation("Cedmin_Idle_008", 0.1f);
			currId = id;
		}
		else if(id == 1 && id != currId) {
			armature->fadeInAnimation("Cedmin_Run_051", 0.1f);		
			currId = id;		
		}
	}
		
	void MiniObjectPlayer::render(sf::RenderTexture &canvas) {
		// sprite.setTexture(texture);
		// sprite.setPosition(pos);
		// sprite.setOrigin(sprite.getLocalBounds().width/2.f, sprite.getLocalBounds().height/2.f);
		// sprite.setScale(scale.x*dir, scale.y);
		// canvas.draw(sprite);
		
		armature->setPosition(pos);
		armature->setScale(scale);
		
		if(isRunning) {
			playAnimation(1);
		}
		else {
			playAnimation(0);
		}		
		
		armature->setFlipX(dir==-1);
		canvas.draw(*armature);
	}
	
	void MiniObjectPlayer::update(float deltaTime) {
		isRunning = false;
		if(input.getManager().isKept(Keyboard::Left)) {
			pos.x -= speed*deltaTime*60.f;
			dir = -1;
			isRunning = true;
		}
		if(input.getManager().isKept(Keyboard::Right)) {
			pos.x += speed*deltaTime*60.f;
			dir = 1;
			isRunning = true;
		}
		if(input.getManager().isKept(Keyboard::Up)) {
			pos.y -= speed*deltaTime*60.f;
			isRunning = true;
		}
		if(input.getManager().isKept(Keyboard::Down)) {
			pos.y += speed*deltaTime*60.f;
			isRunning = true;
		}
		if(funcOnUpdate.valid()) funcOnUpdate(this);
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

	
	bool MiniObjectPlayer::isMoving() {
		return isRunning;
	}

	void MiniObjectPlayer::runFileScript(const std::string& filepath) {
		auto& lua = script.get();
		lua.set("THIS", this);
		try {
			removeScript();
			lua.script_file(filepath);
		}
		catch(sol::error e) {
			std::cout<<"[ERROR] Cannot load "<<filepath<<'\n'<<std::flush;
			std::cout<<e.what()<<'\n'<<std::flush;
		}
	}
	
	void MiniObjectPlayer::runScript(const std::string& code) {
		auto& lua = script.get();
		lua.set("THIS", this);
		try {
			removeScript();
			lua.script(code);
		}
		catch(sol::error e) {
			std::cout<<"[ERROR] Cannot run script \n"<<std::flush;
			std::cout<<e.what()<<'\n'<<std::flush;
		}
	}

	void MiniObjectPlayer::initScript(Script& script) {
		auto object = script.newClass<MiniObjectPlayer>("MiniObjectPlayer", "MiniWorld");
		
		// Base
		object.set("pos", &MiniObject::pos);
		object.set("getName", &MiniObject::getName);
		object.set("setName", &MiniObject::setName);
		
		// Scene
		object.set("loadTexture", &MiniObjectScene::loadTexture);
		object.set("setScale", &MiniObjectScene::setScale);
		object.set("runFileScript", &MiniObjectScene::runFileScript);
		object.set("runScript", &MiniObjectScene::runScript);
		object.set("_onAction", &MiniObjectScene::funcOnAction);
		object.set("_onUpdate", &MiniObjectScene::funcOnUpdate);
		
		// Player		
		object.set("playAnimation", &MiniObjectPlayer::playAnimation);
		object.set("isMoving", &MiniObjectPlayer::isMoving);
		object.set("speed", &MiniObjectPlayer::speed);
		
		object.init();
	}

}