#include "BaseComponent.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Utility/ImGuiTweaks.hpp"

#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include <imgui.h>

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

	void BaseComponent::loadFromConfig(Json& config)
	{
		Component::loadFromConfig(config);
		if(auto& var = config["position_only"]; !var.is_null()) _positionOnly = var;
	}

	void BaseComponent::saveToConfig(Json& config) const
	{
		Component::saveToConfig(config);
		config["position_only"] = _positionOnly;
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
			ImGui::DragVec3<ImGui::CopyPaste, ImGui::Reset>("Position##base_component", position);
			object->setPosition(position);

			// Position only
			if(_positionOnly) return;

			// Set origin
			glm::vec3 origin = object->getOrigin();
			origin.y *= -1;
			ImGui::DragVec3<ImGui::CopyPaste>("Origin##base_component", origin);
			object->setOrigin(origin);

			// Set rotation
			glm::vec3 rotation = object->getRotation();
			ImGui::DragVec3<ImGui::CopyPaste, ImGui::Reset>("Rotation##base_component", rotation);
			object->setRotation(rotation);

			// Set scale
			glm::vec2 scale = object->getScale();
			static bool lockRatio = false;
			if (ImGui::DragVec2<ImGui::CopyPaste>("##Scale|base_component", scale, 0.01f)) {
				if (lockRatio == true) {

					float offset = (scale.x - object->getScale().x) + (scale.y - object->getScale().y);

					scale = object->getScale();
					scale.x += offset;
					scale.y += offset;
				}
			}
			object->setScale(glm::vec3(scale, 1.f));

			ImGui::SameLine(0.f, 0.f);
			ImGui::Checkbox("Scale##base_component", &lockRatio);

			bool isVisible = object->isVisible();
			ImGui::Checkbox("Visible##base_component", &isVisible);
			object->setVisible(isVisible);
		}
	}

	void BaseComponent::positionOnly(bool flag) {
		_positionOnly = flag;
	}

	void BaseComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<BaseComponent>("BaseComponent", "World");

		// Entity
		entity.set("move", [](Entity& entity, float x, float y, float z){entity.move({x,y,z});});
		entity.set("setPosition", [](Entity& entity, float x, float y, float z){entity.setPosition({x,y,z});});
		entity.set("getPosition", [](Entity& entity){return entity.getPosition();});

		entity.set("rotate", [](Entity& entity, float x, float y, float z){entity.rotate({x,y,z});});
		entity.set("setRotation", [](Entity& entity, float x, float y, float z){entity.setRotation({x,y,z});});
		entity.set("getRotation", [](Entity& entity){return entity.getRotation();});

		entity.set("scale", [](Entity& entity, float x, float y, float z){entity.scale({x,y,z});});
		entity.set("setScale", [](Entity& entity, float x, float y, float z){entity.setScale({x,y,z});});
		entity.set("getScale", [](Entity& entity){return entity.getScale();});

		entity.set("setOrigin", [](Entity& entity, float x, float y, float z){entity.setOrigin({x,y,z});});
		entity.set("getOrigin", [](Entity& entity){return entity.getOrigin();});

		entity.set("setVisible", [] (Entity& entity, bool isVisible) { entity.setVisible(isVisible); });
		entity.set("isVisible", [] (Entity& entity) { return entity.isVisible(); });

		object.init();

	}
}
