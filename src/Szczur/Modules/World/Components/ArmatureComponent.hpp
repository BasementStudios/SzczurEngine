#pragma once

#include <memory>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Modules/DragonBones/SF3DArmatureDisplay.hpp"

#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/ArmatureDisplayData.hpp"

namespace rat
{

class Script;

class ArmatureComponent : public Component, public sf3d::Drawable
{
public:

// Constructors

	///
	ArmatureComponent(Entity* parent);

	///
	~ArmatureComponent();

// Getters

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
	virtual std::unique_ptr<Component> copy(Entity* newParent) const;

// Saving and loading

	///
	virtual void loadFromConfig(const Json& config);

	///
	virtual void saveToConfig(Json& config) const;

// Manipulations

	///
	void playAnim(const std::string& animationName, int playTimes = -1);

	///
	void fadeIn(const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1);

	///
	void setFlipX(bool flipX);

	///
	void setSpeed(float speed);

// Scripts

	///
	static void initScript(Script& script);

// Main

	///
	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

private:

	dragonBones::SF3DArmatureDisplay* _armature = nullptr;
	ArmatureDisplayData* _armatureDisplayData = nullptr;
};

}
