#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	struct EditorObject {
		virtual void editor_ObjectFrame(sf::RenderTexture &canvas, int state) = 0;
		
		virtual const sf::Vector2f& getPosition() = 0;
		virtual void setPosition(const sf::Vector2f&) = 0;		
		
		virtual const std::string& getName() = 0;
		virtual void setName(const std::string&) = 0;		
			
		virtual void loadScript(const std::string& filepath) = 0;		
		virtual void runFileScript(const std::string& filepath) = 0;		
		virtual void runScript(const std::string& code) = 0;
	};
}