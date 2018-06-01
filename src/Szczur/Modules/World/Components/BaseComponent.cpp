#include "BaseComponent.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"

#include <ImGui/imgui.h>

namespace rat {
	
	BaseComponent::BaseComponent(Entity* parent)
	: Component { parent, fnv1a_64("BaseComponent"), "BaseComponent" }
	{

	}

	std::unique_ptr<Component> BaseComponent::copy(Entity* newParent) const
	{
		auto ptr = std::make_unique<BaseComponent>(*this);
		ptr->setEntity(newParent);
		return ptr;
	}

	void BaseComponent::loadFromConfig(const Json& config)
	{
		// Component::loadFromConfig(config);
		// auto& spriteDisplayDataHolder = getEntity()->getScene()->getSpriteDisplayDataHolder();
		// auto name = mapUtf8ToWindows1250(config["spriteDisplayData"].get<std::string>());
		// if(name != "") {
		// 	bool found{false};
		// 	for(auto& it : spriteDisplayDataHolder) {
		// 		if(name == it.getName()) {
		// 			setSpriteDisplayData(&it);
		// 			found = true;
		// 		}
		// 	}
		// 	if(!found) {
		// 		try {
		// 			setSpriteDisplayData(&(spriteDisplayDataHolder.emplace_back(name)));
		// 		}
		// 		catch(const std::exception& exc) {

		// 		}
		// 	}
		// }
	}

	void BaseComponent::saveToConfig(Json& config) const
	{
		// Component::saveToConfig(config);
		// config["spriteDisplayData"] = _spriteDisplayData ? mapWindows1250ToUtf8(_spriteDisplayData->getName()) : "";
	}

	void BaseComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Base##base_component", ImGuiTreeNodeFlags_DefaultOpen)) {
			
			// Set name with input text			
			static char name[255];
			std::strcpy(&name[0], object->getName().c_str());
			ImGui::InputText("", name, 255);
			object->setName(name);

			// Set position
			glm::vec3 position = object->getPosition();
			ImGui::DragFloat3("Position##base_component", reinterpret_cast<float*>(&position));
			object->setPosition(position);

			// Set origin
			glm::vec3 origin = object->getOrigin();
			origin.y *= -1;
			ImGui::DragFloat3("Origin##base_component", reinterpret_cast<float*>(&origin));
			object->setOrigin(origin);

			// Set rotation
			glm::vec3 rotation = object->getRotation();
			ImGui::DragFloat3("Rotation##base_component", reinterpret_cast<float*>(&rotation));
			object->setRotation(rotation);

			// Set scale
			glm::vec3 scale = object->getScale();
			static bool lockRatio = false;
			ImGui::DragFloat2("##Scale|base_component", reinterpret_cast<float*>(&scale), 0.01f);
			ImGui::SameLine(0.f, 0.f);
			ImGui::Checkbox("Scale##base_component", &lockRatio);
			if(lockRatio == false) {
				object->setScale(scale);
			}
			else {
				float offset = (scale.x / object->getScale().x) + (scale.y / object->getScale().y) - 1;
				object->scale( {offset, offset, 1.f} );
			}
		}
	}
}
