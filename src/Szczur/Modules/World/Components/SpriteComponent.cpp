#include "SpriteComponent.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include <Szczur/Modules/World/World.hpp>


namespace rat {
	SpriteComponent::SpriteComponent(Entity* parent)
	: Component { parent, fnv1a_64("SpriteComponent"), "SpriteComponent", Component::Drawable }
	{

	}
	///
	std::unique_ptr<Component> SpriteComponent::copy(Entity* newParent) const
	{
		auto ptr = std::make_unique<SpriteComponent>(*this);

		ptr->setEntity(newParent);

		return ptr;
	}

	///
	void SpriteComponent::setSpriteDisplayData(SpriteDisplayData* spriteDisplayData)
	{
		_spriteDisplayData = spriteDisplayData;
	}

	void SpriteComponent::setTexture(const std::string& texturePath) {
		auto* data = detail::globalPtr<World>->getScenes().getTextureDataHolder().getData(texturePath);
		setSpriteDisplayData(data);
	}

	///
	SpriteDisplayData* SpriteComponent::getSpriteDisplayData() const
	{
		return _spriteDisplayData;
	}

	///
	void* SpriteComponent::getFeature(Component::Feature_e feature)
	{
		if (feature == Feature_e::Drawable)	return static_cast<sf3d::Drawable*>(this);

		return nullptr;
	}

	///
	const void* SpriteComponent::getFeature(Component::Feature_e feature) const
	{
		if (feature == Feature_e::Drawable) return static_cast<const sf3d::Drawable*>(this);

		return nullptr;
	}

	///
	void SpriteComponent::loadFromConfig(Json& config)
	{
		Component::loadFromConfig(config);
		// auto& spriteDisplayDataHolder = getEntity()->getScene()->getSpriteDisplayDataHolder();
		auto name = mapUtf8ToWindows1250(config["spriteDisplayData"].get<std::string>());
		if(name != "") {
			// LOG_INFO("A")
			auto& textureDataHolder = getEntity()->getScene()->getScenes()->getTextureDataHolder();
			// LOG_INFO("B")
			auto* data = textureDataHolder.getData(name);
			// LOG_INFO("C")
			setSpriteDisplayData(data);
			// LOG_INFO("D")

			// bool found{false};
			// for(auto& it : spriteDisplayDataHolder) {
			// 	if(name == it.getName()) {
			// 		setSpriteDisplayData(&it);
			// 		found = true;
			// 	}
			// }
			// if(!found) {
			// 	try {
			// 		setSpriteDisplayData(&(spriteDisplayDataHolder.emplace_back(name)));
			// 	}
			// 	catch(const std::exception& exc) {

			// 	}
			// }
		}
	}

	///
	void SpriteComponent::saveToConfig(Json& config) const
	{
		Component::saveToConfig(config);
		config["spriteDisplayData"] = _spriteDisplayData ? mapWindows1250ToUtf8(_spriteDisplayData->getName()) : "";
	}

	///
	void SpriteComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
	{
		// return;
		if(_spriteDisplayData) {
			states.transform *= getEntity()->getTransform();
			target.draw(*_spriteDisplayData, states);
		}
	}

	void SpriteComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Sprite##sprite_component")) {

			// Sprite data holder
			auto& sprites = object->getScene()->getSpriteDisplayDataHolder();

			// Load texture button
			if(ImGui::Button("Load texture...##sprite_component")) {
				
				// Path to .png file
				std::string file = scenes.getRelativePathFromExplorer("Select texture", ".\\Assets");
				
				// Load file to sprite data holder
				if(file != "") {
					auto* data = scenes.getTextureDataHolder().getData(file);
					setSpriteDisplayData(data);
					// try {
					// 	auto& it = sprites.emplace_back(file);
					// 	setSpriteDisplayData(&it);
					// }
					// catch(const std::exception& exc) {
					// 	setSpriteDisplayData(nullptr);

					// 	LOG_EXCEPTION(exc);
					// }
				}
			}

			// Show path to .png file
			ImGui::Text("Path:");
			ImGui::SameLine();
			ImGui::Text(getSpriteDisplayData() ? mapWindows1250ToUtf8(getSpriteDisplayData()->getName()).c_str() : "None");
		}
	}

	void SpriteComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<SpriteComponent>("SpriteComponent", "World");

		// Main
		object.set("setTexture", &SpriteComponent::setTexture);
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

		// Entity
		entity.set("addSpriteComponent", [&](Entity& e){return (SpriteComponent*)e.addComponent<SpriteComponent>();});
		entity.set("sprite", &Entity::getComponentAs<SpriteComponent>);

		object.init();
	}
}
