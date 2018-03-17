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
		ImGui::Separator();
		if(!ImGui::TreeNode("Base")) return;
		ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.5-30);
		
		ImGui::DragFloat("##PosX", &pos->x);
		ImGui::SameLine(); 
		ImGui::DragFloat("##PosY", &pos->y);
		ImGui::SameLine(); 
		ImGui::Text("Pos");
		
		ImGui::DragFloat("##SizeX", &size->x); 
		ImGui::SameLine(); 
		ImGui::DragFloat("##SizeY", &size->y); 
		ImGui::SameLine(); 
		ImGui::Text("Size");
		
		ImGui::PopItemWidth();
		ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::TreePop();
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