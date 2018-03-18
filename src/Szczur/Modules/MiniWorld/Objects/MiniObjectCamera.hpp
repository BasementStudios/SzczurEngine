#pragma once

#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

struct MiniObjectCamera : public MiniObject {
	
	Input& input;
	Window *window;
	
	sf::Vector2f *target = nullptr;
	sf::View view;

	// bool moveArrows;
	
	MiniObjectCamera(Script &script, Input &input);
	
/////////////////////////// METHODS ///////////////////////////

	void setTarget(sf::Vector2f& target);
	
/////////////////////////// MAIN METHODS ///////////////////////////
	void update(float deltaTime);
	void editor();
	void render(sf::RenderTexture &canvas);
	void editorRender(sf::RenderTexture &canvas);
	
/////////////////////////// SCRIPT ///////////////////////////
	void runFileScript(const std::string& filepath){}	
	void runScript(const std::string& code){}
	// static void initScript(Script& script);
};
	
}