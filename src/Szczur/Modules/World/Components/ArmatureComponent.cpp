#include "ArmatureComponent.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Utility/ImGuiTweaks.hpp"
#include "Szczur/Modules/Script/Script.hpp"

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
	auto armature = std::find_if(armatures.begin(), armatures.end(), [&] (auto& armature) { return armature->getFolderPath() == directory; });

	ArmatureDisplayData* it = nullptr;

	// if not found create new
	if (armature == armatures.end())
	{
		try
		{
			it = armatures.emplace_back(std::move(std::make_unique<ArmatureDisplayData>(directory))).get();
		}
		catch (std::exception& ex)
		{
			LOG_EXCEPTION(ex);
			return;
		}
	}
	else
	{
		it = armature->get();
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

	if (_armature)
	{
		config["speed"] = _armature->getAnimation()->timeScale;

		if (_armature->getAnimation()->isPlaying())
		{
			config["animationMame"] = _armature->getAnimation()->getLastAnimationName();
		}
	}
}

void ArmatureComponent::update(ScenesManager& scenes, float deltaTime)
{
	if (_armature)
	{
		_armature->getArmature()->advanceTime(deltaTime);

		if (_onceAnimStatus == OnceAnimStatus::IsAboutToPlay)
		{
			auto anim = _armature->getAnimation();

			if (anim->isCompleted())
			{
				anim->fadeIn(_playOnceAnimationName, _playOnceAnimationFadeInTime, 1);
				anim->timeScale = _playOnceAnimationSpeed;
				_onceAnimStatus = OnceAnimStatus::IsPlaying;
			}
		}

		if (_onceAnimStatus == OnceAnimStatus::IsPlaying)
		{
			auto anim = _armature->getAnimation();

			if (anim->isCompleted())
			{
				fadeIn(_lastAnimationName, _lastAnimationFadeInTime);
				anim->timeScale = _lastAnimationSpeed;

				_onceAnimStatus = OnceAnimStatus::None;
			}
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

void ArmatureComponent::loadArmature()
{
	if (_armatureDisplayData)
	{
		setArmatureDisplayData(_armatureDisplayData);

		if (_armature && _armature->getAnimation() && !_lastPlayingAnimation.empty())
		{
			_armature->getAnimation()->play(_lastPlayingAnimation);
		}
	}
}

void ArmatureComponent::unloadArmature()
{
	if (_armature)
	{
		if (_armature->getAnimation())
		{
			_lastPlayingAnimation = _armature->getAnimation()->getLastAnimationName();
		}

		delete _armature;
		_armature = nullptr;
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

		_onceAnimStatus = OnceAnimStatus::None;
		_armature->getAnimation()->fadeIn(animationName, fadeInTime, playTimes);
	}
}

void ArmatureComponent::playOnce(const std::string& animationName, float fadeInTime, bool waitToEnd, float animationSpeed)
{
	if (_armature)
	{
		if (_onceAnimStatus == OnceAnimStatus::IsPlaying)
			return;

		if (auto anim = _armature->getAnimation(); anim && anim->isPlaying())
		{
			if (auto lastAnimation = anim->getLastAnimationState())
			{
				if (_onceAnimStatus == OnceAnimStatus::None)
				{
					_lastAnimationName = lastAnimation->name;
					_lastAnimationFadeInTime = lastAnimation->fadeTotalTime;
					_lastAnimationSpeed = anim->timeScale;
				}

				if (waitToEnd)
				{
					_onceAnimStatus = OnceAnimStatus::IsAboutToPlay;
					_playOnceAnimationName = animationName;
					_playOnceAnimationFadeInTime = fadeInTime;
					_playOnceAnimationSpeed = animationSpeed;

					auto time = lastAnimation->getCurrentTime();
					anim->gotoAndPlayByTime(lastAnimation->getName(), time, 1);
				}
				else
				{
					_onceAnimStatus = OnceAnimStatus::IsPlaying;
					anim->fadeIn(animationName, fadeInTime, 1);
					anim->timeScale = animationSpeed;
				}
			}
		}
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

void ArmatureComponent::replaceSkin(const std::string& skinName, sol::variadic_args excludes)
{
	auto skinData = dragonBones::SF3DFactory::get()->getArmatureData(skinName)->defaultSkin;

	if (skinData == nullptr)
		return;

	std::vector<std::string> slotsToExclude;

	for (auto slot : excludes)
	{
		slotsToExclude.push_back(slot.get<std::string>());
	}

	dragonBones::SF3DFactory::get()->replaceSkin(_armature->getArmature(), skinData, false, &slotsToExclude);
}

void ArmatureComponent::setSlotDisplayIndex(const std::string& slotName, int displayIndex)
{
	auto slot = _armature->getArmature()->getSlot(slotName);

	if (slot == nullptr)
		return;

	slot->setDisplayIndex(displayIndex);
}

void ArmatureComponent::setSlotDisplay(const std::string& slotName, const std::string& displayName)
{
	auto slot = _armature->getArmature()->getSlot(slotName);

	if (slot == nullptr)
		return;

	auto& displays = *slot->getRawDisplayDatas();

	auto display = std::find_if(displays.begin(), displays.end(), [&] (dragonBones::DisplayData* displayData) { return displayData->name == displayName; });

	if (display != displays.end())
	{
		auto index = std::distance(displays.begin(), display);

		slot->setDisplayIndex(static_cast<int>(index));
	}
}

bool ArmatureComponent::isPlaying()
{
	if (_armature)
	{
		return _armature->getAnimation()->isPlaying();
	}

	return false;
}

std::string ArmatureComponent::getCurrentPlayingAnim()
{
	if (_armature && _armature->getAnimation())
	{
		return _armature->getAnimation()->getLastAnimationName();
	}

	return std::string();
}

void ArmatureComponent::initScript(ScriptClass<Entity>& entity, Script& script)
{
	auto object = script.newClass<ArmatureComponent>("ArmatureComponent", "World");

	// Main
	object.set("play", &ArmatureComponent::playAnim);
	object.set("fadeIn", &ArmatureComponent::fadeIn);


	object.set("playOnce", sol::overload(
		&ArmatureComponent::playOnce,
		[&] (ArmatureComponent* comp, const std::string& animationName, float fadeInTime, bool waitToEnd) { comp->playOnce(animationName, fadeInTime, waitToEnd); },
		[&] (ArmatureComponent* comp, const std::string& animationName, float fadeInTime) { comp->playOnce(animationName, fadeInTime); }
	));


	object.set("setFlipX", &ArmatureComponent::setFlipX);
	object.set("setSpeed", &ArmatureComponent::setSpeed);
	object.set("isPlaying", &ArmatureComponent::isPlaying);
	object.set("getCurrentPlayingAnim", &ArmatureComponent::getCurrentPlayingAnim);
	object.set("setArmature", &ArmatureComponent::setArmature);
	object.set("replaceSkin", &ArmatureComponent::replaceSkin);
	object.set("setSlotDisplayIndex", &ArmatureComponent::setSlotDisplayIndex);
	object.set("setSlotDisplay", &ArmatureComponent::setSlotDisplay);
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
		Component::drawOriginSetter<ArmatureComponent>(&ArmatureComponent::setOrigin);

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
		if (getArmatureDisplayData())
		{
			ImGui::SameLine();
			if (ImGui::Button("Change entity name"))
			{
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

			ImGui::DragFloat<ImGui::CopyPaste>("Animation speed##armature_component", arm->getAnimation()->timeScale, 0.01f);
		}
	}
}

void ArmatureComponent::setOrigin(int vertical, int horizontal)
{
	if (!_armature || !_armature->getArmature())
		return;

	auto bb = _armature->getBoundingBox();

	glm::vec2 pos;

	switch (vertical)
	{
		case -1:
			pos.x = bb.left;
			break;
		case 0:
			pos.x = 0;
			break;
		case 1:
			pos.x = bb.left + bb.width;
			break;
	}

	switch (horizontal)
	{
		case -1:
			pos.y = -(bb.top + bb.height);
			break;
		case 0:
			pos.y = 0;
			break;
		case 1:
			pos.y = bb.height - (bb.top + bb.height);
			break;
	}

	getEntity()->setOrigin(glm::vec3(pos, getEntity()->getOrigin().z));
}

}