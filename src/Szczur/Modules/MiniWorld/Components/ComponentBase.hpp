#pragma once

#include <SFML/Graphics.hpp>

#include <Szczur/Modules/Script/Script.hpp>

#include "Component.hpp"

namespace rat {
	struct MiniObject;

	struct ComponentBase : public Component {
		
		const std::string componentName = "Base";
		MiniObject* target = nullptr;
		sf::Vector2f *pos = nullptr;
		sf::Vector2f *size = nullptr;
		
		ComponentBase(MiniObject* target);
			
		const std::string& getComponentName();
		
		void update();		
		void editor();	
		void render(sf::RenderTexture& canvas);		
		static void initScript(Script& script);
	};
}