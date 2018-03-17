#include "ComponentBase.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

	ComponentBase::ComponentBase(MiniObject* target) 
		: target(target), pos(&target->pos), size(&target->size) {
	}	
	
	const std::string& ComponentBase::getComponentName() {
		return componentName;		
	}
	
	void ComponentBase::update() {
	}
	
	void ComponentBase::editor() {
	}
	
	void ComponentBase::render(sf::RenderTexture& canvas) {
	}

	void ComponentBase::initScript(Script& script) {
		auto object = script.newClass<ComponentBase>("ComponentBase", "MiniWorld");
		object.set("pos", &ComponentBase::pos);
		object.set("size", &ComponentBase::size);
		object.init();
	}
}