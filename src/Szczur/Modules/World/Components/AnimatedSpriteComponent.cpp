#include "AnimatedSpriteComponent.hpp"

#include <experimental/filesystem>

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include <Szczur/Modules/World/World.hpp>


namespace rat {
	AnimatedSpriteComponent::AnimatedSpriteComponent(Entity* parent)
		: Component { parent, fnv1a_64("AnimatedSpriteComponent"), "AnimatedSpriteComponent", Component::Drawable }
	{

	}

	std::unique_ptr<Component> AnimatedSpriteComponent::copy(Entity* newParent) const
	{
		auto ptr = std::make_unique<AnimatedSpriteComponent>(*this);

		ptr->setEntity(newParent);

		return ptr;
	}

	void AnimatedSpriteComponent::setSpriteDisplayData(SpriteDisplayData* spriteDisplayData)
	{
		_spriteDisplayData = spriteDisplayData;
	}

	void AnimatedSpriteComponent::setTexture(const std::string& texturePath)
	{
		auto* data = detail::globalPtr<World>->getScenes().getTextureDataHolder().getData(texturePath);
		setSpriteDisplayData(data);
	}

	SpriteDisplayData* AnimatedSpriteComponent::getSpriteDisplayData() const
	{
		return _spriteDisplayData;
	}

	void* AnimatedSpriteComponent::getFeature(Component::Feature_e feature)
	{
		if (feature == Feature_e::Drawable)	return static_cast<sf3d::Drawable*>(this);

		return nullptr;
	}

	const void* AnimatedSpriteComponent::getFeature(Component::Feature_e feature) const
	{
		if (feature == Feature_e::Drawable) return static_cast<const sf3d::Drawable*>(this);

		return nullptr;
	}

	void AnimatedSpriteComponent::loadFromConfig(Json& config)
	{
		Component::loadFromConfig(config);

		auto name = mapUtf8ToWindows1250(config["spriteDisplayData"].get<std::string>());

		if (!name.empty())
		{
			auto& textureDataHolder = getEntity()->getScene()->getScenes()->getTextureDataHolder();
			auto* data = textureDataHolder.getData(name);
			setSpriteDisplayData(data);
		}

		_frames = config["frames"];

		_autoUpdateFrames = config["autoUpdateFrames"];

		_frameSize.x = config["frameSize"]["x"];
		_frameSize.y = config["frameSize"]["y"];
		_autoUpdateFrameSize = config["autoUpdateFrameSize"];

		_rows = config["rows"];
		_columns = config["columns"];
		_speed = config["speed"];
	}

	void AnimatedSpriteComponent::saveToConfig(Json& config) const
	{
		Component::saveToConfig(config);
		config["spriteDisplayData"] = _spriteDisplayData ? mapWindows1250ToUtf8(_spriteDisplayData->getName()) : "";

		config["frames"] = _frames;
		config["autoUpdateFrames"] = _autoUpdateFrames;

		config["frameSize"]["x"] = _frameSize.x;
		config["frameSize"]["y"] = _frameSize.y;
		config["autoUpdateFrameSize"] = _autoUpdateFrameSize;

		config["rows"] = _rows;
		config["columns"] = _columns;

		config["speed"] = _speed;
	}

	void AnimatedSpriteComponent::update(ScenesManager& scenes, float deltaTime)
	{
		if (_spriteDisplayData == nullptr)
			return;

		// 30 frames per sec
		if (_lastUpdate.getElapsedTime().asSeconds() < 30 / 1000.f / _speed)
			return;

		int x = _currentFrame % _columns;
		int y = _currentFrame / _columns;

		//printf("[%d] X: %d, Y: %d\n", _currentFrame, x, y);

		sf::FloatRect rect(x * _frameSize.x, y * _frameSize.y, _frameSize.x, _frameSize.y);
		setTextureRect(rect);

		++_currentFrame;

		if (_currentFrame >= _frames)
			_currentFrame = 0;

		_lastUpdate.restart();
	}

	void AnimatedSpriteComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
	{
		if(_spriteDisplayData)
		{
			states.transform *= getEntity()->getTransform();
			states.texture = &_spriteDisplayData->getTexture();

			target.draw(_vertices, states);
		}
	}

	void AnimatedSpriteComponent::setTextureRect(const sf::FloatRect& rect)
	{
		auto texSize = _spriteDisplayData->getTexture().getSize();

		_vertices[0] = {
			{ 0.f, 0.f, 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ rect.left / texSize.x, rect.top / texSize.y }
		};
		_vertices[1] = {
			{ rect.width, 0.f, 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ (rect.left + rect.width) / texSize.x, rect.top / texSize.y }
		};
		_vertices[2] = {
			{ rect.width, -rect.height, 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ (rect.left + rect.width) / texSize.x, (rect.top + rect.height) / texSize.y }
		};
		_vertices[3] = {
			{ 0.f, -rect.height, 0.f },
			{ 1.f, 1.f, 1.f, 1.f },
			{ rect.left / texSize.x, (rect.top + rect.height) / texSize.y }
		};
	}

	void AnimatedSpriteComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Animated Sprite##animated_sprite_component")) 
		{
			// Sprite data holder
			auto& sprites = object->getScene()->getSpriteDisplayDataHolder();

			// Load texture button
			if(ImGui::Button("Load texture...")) {
				
				// Path to .png file
			    std::string file = scenes.getRelativePathFromExplorer("Select texture", ".\\Assets", "Images (*.png, *.jpg, *.psd|*.png;*.jpg;*.psd");
			    
				// Load file to sprite data holder
				if(file != "") {
					auto* data = scenes.getTextureDataHolder().getData(file);
					setSpriteDisplayData(data);
					_lastUpdate.restart();
				}
			}

			// Change entity name
			if(getSpriteDisplayData()) {
				ImGui::SameLine();
				if(ImGui::Button("Change entity name")) {
					getEntity()->setName(std::experimental::filesystem::path(getSpriteDisplayData()->getName()).stem().string());
				}
			}

			// Show path to .png file
			ImGui::Text("Path:");
			ImGui::SameLine();
			ImGui::Text(getSpriteDisplayData() ? mapWindows1250ToUtf8(getSpriteDisplayData()->getName()).c_str() : "None");

			ImGui::Separator();

			ImGui::Checkbox("Auto update frames", &_autoUpdateFrames);
			ImGui::InputInt("Frames", &_frames);

			if (ImGui::InputInt("Columns", &_columns))
			{
				if (_spriteDisplayData && _columns != 0 && _rows != 0)
				{
					if (_autoUpdateFrameSize)
					{
						auto size = _spriteDisplayData->getTexture().getSize();
						_frameSize = { size.x / _columns,  size.y / _rows };
					}

					if (_autoUpdateFrames)
					{
						_frames = _columns * _rows;
					}
				}
			}

			if (ImGui::InputInt("Rows", &_rows))
			{
				if (_spriteDisplayData && _columns != 0 && _rows != 0)
				{
					if (_autoUpdateFrameSize)
					{
						auto size = _spriteDisplayData->getTexture().getSize();
						_frameSize = { size.x / _columns,  size.y / _rows };
					}

					if (_autoUpdateFrames)
					{
						_frames = _columns * _rows;
					}
				}
			}

			ImGui::Checkbox("Auto update size", &_autoUpdateFrameSize);
			ImGui::InputFloat2("Frame size", &_frameSize[0], 1);

			ImGui::DragFloat("Speed", &_speed, 0.05f, 0.1f, 100.f);
		}
	}

	void AnimatedSpriteComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<AnimatedSpriteComponent>("SpriteComponent", "World");

		// Main

		// getters and setters
		object.set("setFrameNum", &AnimatedSpriteComponent::setFrameNum);
		object.set("getFrameNum", &AnimatedSpriteComponent::getFrameNum);

		object.set("setFrameSize", &AnimatedSpriteComponent::setFrameSize);
		object.set("getFrameSize", &AnimatedSpriteComponent::getFrameSize);

		object.set("setRows", &AnimatedSpriteComponent::setRows);
		object.set("getRows", &AnimatedSpriteComponent::getRows);

		object.set("setColumns", &AnimatedSpriteComponent::setColumns);
		object.set("getColumns", &AnimatedSpriteComponent::getColumns);

		object.set("setSpeed", &AnimatedSpriteComponent::setSpeed);
		object.set("getSpeed", &AnimatedSpriteComponent::getSpeed);


		// Texture
		object.set("setTexture", &AnimatedSpriteComponent::setTexture);
		object.set("setTextureData", &AnimatedSpriteComponent::setSpriteDisplayData);
		object.set("getTextureData", &AnimatedSpriteComponent::getSpriteDisplayData);
		object.set("getTextureSize", [](AnimatedSpriteComponent& comp){return glm::vec2(comp._spriteDisplayData->getTexture().getSize());});

		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));


		// Entity
		entity.set("addAnimatedSpriteComponent", [&](Entity& e){return (AnimatedSpriteComponent*)e.addComponent<AnimatedSpriteComponent>();});
		entity.set("animatedSprite", &Entity::getComponentAs<AnimatedSpriteComponent>);


		object.init();
	}
}
