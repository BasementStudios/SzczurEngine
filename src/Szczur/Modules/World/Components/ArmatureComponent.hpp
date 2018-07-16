#pragma once

#include <memory>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Modules/DragonBones/SF3DArmatureDisplay.hpp"

#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/ArmatureDisplayData.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace dragonBones
{
	class AnimationState;
}

namespace rat
{

class Entity;
template<class T> class ScriptClass;

class ArmatureComponent : public Component, public sf3d::Drawable
{
private:
	enum OnceAnimStatus
	{
		IsAboutToPlay,
		IsPlaying,
		None
	};

public:

// Constructors

	///
	ArmatureComponent(Entity* parent);

	///
	~ArmatureComponent();

// Getters and setters

	///
	ArmatureDisplayData* getArmatureDisplayData();

	///
	const ArmatureDisplayData* getArmatureDisplayData() const;

	///
	dragonBones::SF3DArmatureDisplay* getArmature();

	///
	virtual void* getFeature(Component::Feature_e feature) override;

	///
	virtual const void* getFeature(Component::Feature_e feature) const override;
	
	///
	void setArmatureDisplayData(ArmatureDisplayData* armatureDisplayData, bool deleteOld = true);

	///
	void setArmature(const std::string& armatureName);

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const;

// Saving and loading

	///
	virtual void loadFromConfig(Json& config);

	///
	virtual void saveToConfig(Json& config) const;

// Manipulations

	///
	void loadArmature();

	///
	void unloadArmature();

	///
	void playAnim(const std::string& animationName, int playTimes = -1);

	///
	void fadeIn(const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1);

	///
	void playOnce(const std::string& animationName, float fadeInTime = -1.f, bool waitToEnd = false, float animationSpeed = 1.f);

	///
	void setFlipX(bool flipX);

	///
	void setSpeed(float speed);

	///
	void replaceSkin(const std::string& skinName, sol::variadic_args excludes);

	///
	void setSlotDisplayIndex(const std::string& slotName, int displayIndex);

	///
	void setSlotDisplay(const std::string& slotName, const std::string& displayName);

	///
	bool isPlaying();
	
	///
	std::string getCurrentPlayingAnim();

// Scripts

	///
	static void initScript(ScriptClass<Entity>& entity, Script& script);

// Main

	///
	void update(ScenesManager& scenes, float deltaTime);

	///
	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

private:
	// @vertical: -1 left, 0 center, 1 right
	// @horizontal: -1 top, 0 center, 1 bottom
	void setOrigin(int vertical = 0, int horizontal = 0);

private:
	dragonBones::SF3DArmatureDisplay* _armature = nullptr;
	ArmatureDisplayData* _armatureDisplayData = nullptr;

	OnceAnimStatus _onceAnimStatus;

	std::string _playOnceAnimationName;
	float _playOnceAnimationFadeInTime = 0.f;
	float _playOnceAnimationSpeed = 1.f;

	std::string _lastAnimationName;
	float _lastAnimationFadeInTime = 0.f;
	float _lastAnimationSpeed = 1.f;

	std::string _lastPlayingAnimation;
};

}
