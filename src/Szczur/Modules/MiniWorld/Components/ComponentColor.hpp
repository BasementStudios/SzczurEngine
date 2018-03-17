#pragma once

#include <SFML/Graphics.hpp>

#include <Szczur/Modules/Script/Script.hpp>

#include "Component.hpp"

namespace rat {
	struct MiniObject;

	struct ComponentColor : public Component {
		
		const std::string componentName = "Color";
		MiniObject* target = nullptr;		
		int color = 0;
		
		ComponentColor(MiniObject* target);
			
		const std::string& getComponentName();
		
		void update();		
		void editor();	
		void render(sf::RenderTexture& canvas);		
		static void initScript(Script& script);
	};
}