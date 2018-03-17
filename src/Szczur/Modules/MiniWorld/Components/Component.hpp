#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	
	struct Script;

	struct Component {		
		virtual const std::string& getComponentName() = 0;	
		virtual void update() = 0;		
		virtual void editor() = 0;		
		virtual void render(sf::RenderTexture& canvas) = 0;
		// virtual ~ComponentBase() = 0;
	};
}