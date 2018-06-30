#include "ArmatureComponent.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat
{

ArmatureComponent::ArmatureComponent(Entity* parent) :
	Component(parent, fnv1a_64("ArmatureComponent"), "ArmatureComponent", Component::Drawable)
{

}

ArmatureComponent::~ArmatureComponent()
{
	if (_armature)
		delete _armature;
}

void ArmatureComponent::setArmatureDisplayData(ArmatureDisplayData* armatureDisplayData, bool deleteOld)
{
	_armatureDisplayData = armatureDisplayData;
	if (armatureDisplayData)
	{
		auto dbFactory = dragonBones::SF3DFactory::get();

		if (_armature && deleteOld)
			delete _armature;

		_armature = dbFactory->buildArmatureDisplay(armatureDisplayData->getName());
	}
	else if (_armature)
	{
		if (deleteOld)
			delete _armature;

		_armature = nullptr;
	}
}

void ArmatureComponent::setArmature(const std::string& armatureName)
{
	if (armatureName.empty())
		return;

	auto directory = "Assets\\Armatures\\" + armatureName;

	auto& armatures = getEntity()->getScene()->getScenes()->getArmatureDisplayDataHolder();

	// find armature
	auto armature = std::find_if(armatures.begin(), armatures.end(), [&] (auto& armature) { return armature.getFolderPath() == directory; });

	ArmatureDisplayData* it = nullptr;

	// if not found create new
	if (armature == armatures.end())
	{
		try
		{
			it = &armatures.emplace_back(directory);
		}
		catch (std::exception& ex)
		{
			LOG_EXCEPTION(ex);
			return;
		}
	}
	else
	{
		it = &(*armature);
	}

	// try set armature data
	try
	{
		setArmatureDisplayData(it);
	}
	catch (std::exception& ex)
	{
		setArmatureDisplayData(nullptr, false);
		LOG_EXCEPTION(ex);
	}
}

ArmatureDisplayData* ArmatureComponent::getArmatureDisplayData()
{
	return _armatureDisplayData;
}

const ArmatureDisplayData* ArmatureComponent::getArmatureDisplayData() const
{
	return _armatureDisplayData;
}

dragonBones::SF3DArmatureDisplay* ArmatureComponent::getArmature()
{
	return _armature;
}

std::unique_ptr<Component> ArmatureComponent::copy(Entity* newParent) const
{
	auto ptr = std::make_unique<ArmatureComponent>(*this);

	// prevent deleting
	ptr->_armature = nullptr;
	ptr->setArmatureDisplayData(this->_armatureDisplayData);

	ptr->setEntity(newParent);

	return ptr;
}

void* ArmatureComponent::getFeature(Component::Feature_e feature)
{
	if (feature == Component::Drawable) return static_cast<sf3d::Drawable*>(this);

	return nullptr;
}

const void* ArmatureComponent::getFeature(Component::Feature_e feature) const
{
	if (feature == Component::Drawable) return static_cast<const sf3d::Drawable*>(this);

	return nullptr;
}

void ArmatureComponent::loadFromConfig(Json& config)
{
	Component::loadFromConfig(config);
	auto name = mapUtf8ToWindows1250(config["armatureDisplayData"].get<std::string>());
	if (name != "")
	{
		setArmature(name);

		if (_armature && _armature->getAnimation())
		{
			if (config.find("animationMame") != config.end())
				_armature->getAnimation()->play(config["animationMame"].get<std::string>());

			if (config.find("speed") != config.end())
				_armature->getAnimation()->timeScale = config["speed"];
		}
	}
}

void ArmatureComponent::saveToConfig(Json& config) const
{
	Component::saveToConfig(config);
	config["armatureDisplayData"] = _armatureDisplayData ? mapWindows1250ToUtf8(_armatureDisplayData->getName()) : "";
	config["animationMame"] = _armature ? _armature->getAnimation()->getLastAnimationName() : "";
	config["speed"] = _armature ? _armature->getAnimation()->timeScale : 1.f;
}

void ArmatureComponent::update(ScenesManager& scenes, float deltaTime)
{
	if (_armature)
	{
		_armature->getArmature()->advanceTime(deltaTime);
	}

	if (_isPlayingOnceAnimation)
	{
		if (_armature && _armature->getAnimation()->isCompleted())
		{
			_isPlayingOnceAnimation = false;
			fadeIn(_lastAnimationName, _lastAnimationFadeInTime);
		}
	}
}

void ArmatureComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	if (_armature)
	{
		states.transform *= getEntity()->getTransform();
		_armature->draw(target, states);
	}
}

void ArmatureComponent::playAnim(const std::string& animationName, int playTimes)
{
	if (_armature)
	{
		_armature->getAnimation()->play(animationName, playTimes);
	}
}

void ArmatureComponent::fadeIn(const std::string& animationName, float fadeInTime, int playTimes)
{
	if (_armature)
	{
		if (animationName == _armature->getAnimation()->getLastAnimationName())
		{
			return;
		}

		_isPlayingOnceAnimation = false;
		_armature->getAnimation()->fadeIn(animationName, fadeInTime, playTimes);
	}
}

void ArmatureComponent::playOnce(const std::string& animationName, float fadeInTime)
{
	if (_armature)
	{
		if (_isPlayingOnceAnimation)
		{
			_armature->getAnimation()->fadeIn(_lastAnimationName, _lastAnimationFadeInTime);
		}

		if (auto anim = _armature->getAnimation(); anim && anim->isPlaying())
		{
			auto lastAnimation = anim->getLastAnimationState();
			_lastAnimationFadeInTime = lastAnimation->fadeTotalTime;
			_lastAnimationName = lastAnimation->name;
			_isPlayingOnceAnimation = true;
		}

		_armature->getAnimation()->fadeIn(animationName, fadeInTime, 1);
	}
}

void ArmatureComponent::setFlipX(bool flipX)
{
	if (_armature)
	{
		_armature->getArmature()->setFlipX(flipX);
	}
}

void ArmatureComponent::setSpeed(float speed)
{
	if (_armature)
	{
		_armature->getAnimation()->timeScale = speed;
	}
}

bool ArmatureComponent::isPlaying()
{
	if (_armature)
	{
		return _armature->getAnimation()->isPlaying();
	}
}

void ArmatureComponent::initScript(ScriptClass<Entity>& entity, Script& script)
{
	auto object = script.newClass<ArmatureComponent>("ArmatureComponent", "World");

	// Main
	object.set("play", &ArmatureComponent::playAnim);
	object.set("fadeIn", &ArmatureComponent::fadeIn);
	object.set("playOnce", &ArmatureComponent::playOnce);
	object.set("setFlipX", &ArmatureComponent::setFlipX);
	object.set("setSpeed", &ArmatureComponent::setSpeed);
	object.set("isPlaying", &ArmatureComponent::isPlaying);
	object.set("setArmature", &ArmatureComponent::setArmature);
	object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

	// Entity
	entity.set("armature", &Entity::getComponentAs<ArmatureComponent>);
	entity.set("addArmatureComponent", [&] (Entity& e) {return (ArmatureComponent*)e.addComponent<ArmatureComponent>(); });

	object.init();
}

void ArmatureComponent::renderHeader(ScenesManager& scenes, Entity* object)
{
	if (ImGui::CollapsingHeader("Armature##armature_component"))
	{
		//Component::drawOriginSetter(&ArmatureComponent::setOrigin);
		Component::drawOriginSetter<ArmatureComponent>(&ArmatureComponent::setOrigin);

		// Armature data holder
		auto& armatures = scenes.getArmatureDisplayDataHolder();

		// Load armature button
		if (ImGui::Button("Load armature...##armature_component"))
		{
			// Path to direcotry with armature files
			std::string directory = scenes.getRelativePathFromExplorer("Select armature file", ".\\Assets");

			if (!directory.empty())
			{
				setArmature(std::experimental::filesystem::path(directory).parent_path().filename().string());
			}
		}

		// Change entity name
		if(getArmatureDisplayData()) {
				ImGui::SameLine();
			if(ImGui::Button("Change entity name")) {
				getEntity()->setName(std::experimental::filesystem::path(getArmatureDisplayData()->getName()).stem().string());
			}
		}

		// Show path to armature directory
		ImGui::Text("Path:");
		ImGui::SameLine();
		ImGui::Text(getArmatureDisplayData() ? mapWindows1250ToUtf8(getArmatureDisplayData()->getName()).c_str() : "None");

		// Select animation button
		if (auto* arm = getArmature(); arm && arm->getAnimation())
		{
			if (ImGui::BeginCombo("Animation", arm->getAnimation()->getLastAnimationName().c_str()))
			{
				for (auto& anim : arm->getArmature()->getAnimation()->getAnimationNames())
				{
					bool isSelected = (arm->getAnimation()->getLastAnimationName() == anim);

					if (ImGui::Selectable(anim.c_str(), isSelected))
						arm->getAnimation()->play(anim);
					else
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (arm->getAnimation()->isPlaying())
			{
				ImGui::SameLine();
				if (ImGui::Button("Stop##armature_component"))
				{
					arm->getAnimation()->stop(arm->getAnimation()->getLastAnimationName());
					arm->getAnimation()->reset();
				}
			}

			ImGui::DragFloat("Animation speed##armature_component", &arm->getAnimation()->timeScale, 0.01f);
		}
	}
}

void ArmatureComponent::setOrigin(int vertical, int horizontal)
{
	if (!_armature || !_armature->getArmature())
		return;

	auto size = _armature->getBoundingBox();

	glm::vec2 pos;

	switch (vertical)
	{
		case -1:
			pos.x = -size.width / 2.f;
			break;
		case 0:
			pos.x = 0;
			break;
		case 1:
			pos.x = size.width / 2.f;
			break;
	}

	switch (horizontal)
	{
		case -1:
			pos.y = -size.height / 2.f;
			break;
		case 0:
			pos.y = 0;
			break;
		case 1:
			pos.y = size.height / 2.f;
			break;
	}

	getEntity()->setOrigin(glm::vec3(pos, getEntity()->getOrigin().z));
}

}
