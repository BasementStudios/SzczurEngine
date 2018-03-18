#include "MiniObjectCamera.hpp"

namespace rat {

	MiniObjectCamera::MiniObjectCamera(Script &script, Input &input) 
		: MiniObject(script), input(input) {
		
		window = modulePtr_v<Window>;
		
		// view.setViewport(sf::FloatRect(0, 0, 1, 1));
		type = Types::Camera;
		
		name = "Camera";
		// loadTexture("assets/textures/player.png");
		// setScale(0.1,0.1);
		// armature = modulePtr_v<DragonBones>->createArmature("Cedmin");
		// armature->playAnimation("Cedmin_Run_051");
	}	
	
/////////////////////////// METHODS ///////////////////////////

	void MiniObjectCamera::setTarget(sf::Vector2f& _target) {
		target = &_target;
	}
	
/////////////////////////// MAIN METHODS ///////////////////////////

	void MiniObjectCamera::editorRender(sf::RenderTexture& canvas) {
		sf::CircleShape shape;
		shape.setRadius(10);
		shape.setFillColor({70, 120, 200, 70});
		shape.setOutlineColor({70, 120, 200, 180});
		shape.setOutlineThickness(2);
		shape.setPosition(pos);
		shape.setOrigin(10, 10);
		canvas.draw(shape);
	}
		
	void MiniObjectCamera::render(sf::RenderTexture &canvas) {
		if(target) pos = *target;
		view.setCenter(pos);
		// window->getWindow().setView(view);		
	}
	
	void MiniObjectCamera::update(float deltaTime) {
		
	}
	
	void MiniObjectCamera::editor() {
		ImGui::Text("Camera");
	}
	
/////////////////////////// SCRIPT ///////////////////////////


}