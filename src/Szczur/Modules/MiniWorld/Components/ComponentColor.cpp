#include "ComponentColor.hpp"

#include "Szczur/Modules/MiniWorld/MiniObject.hpp"

namespace rat {

	ComponentColor::ComponentColor(MiniObject* target) 
		: target(target) {
	}
	
	const std::string& ComponentColor::getComponentName() {
		return componentName;		
	}
	
	void ComponentColor::update() {
	}
	
	void ComponentColor::editor() {
		ImGui::Separator();
		if(!ImGui::TreeNode("Color")) return;
		ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.5-30);
		
		ImGui::SliderInt("Color", &color, 0, 2);
	
		ImGui::PopItemWidth();
		ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::TreePop();
	}
	
	void ComponentColor::render(sf::RenderTexture& canvas) {
		if(color == 0) {
			target->shape.setFillColor({100,50,50,255});		
		}
		else if(color == 1) {
			target->shape.setFillColor({140,70,50,255});		
		}
		else if(color == 2) {
			target->shape.setFillColor({180,90,60,255});		
		}
	}

	void ComponentColor::initScript(Script& script) {
		auto object = script.newClass<ComponentColor>("ComponentColor", "MiniWorld");
		object.set("color", &ComponentColor::color);
		object.init();
	}
}