#pragma once

#include <memory>

#include "Szczur/Modules/DragonBones/SF3DArmatureDisplay.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Component.hpp"

#include "ArmatureDisplayData.hpp"

namespace rat
{

class ArmatureComponent : public Component, public sf3d::Drawable
{
private:
	dragonBones::SF3DArmatureDisplay* _armature{nullptr};
	ArmatureDisplayData* _armatureDisplayData;

public:
	ArmatureComponent(ArmatureDisplayData* armatureDisplayData = nullptr);
	~ArmatureComponent();

	void setArmatureDisplayData(ArmatureDisplayData* armatureDisplayData);
	ArmatureDisplayData* getArmatureDisplayData();
	const ArmatureDisplayData* getArmatureDisplayData() const;
	dragonBones::SF3DArmatureDisplay* getArmature();

	virtual std::unique_ptr<Component> copy() const;

	virtual void* getFeature(size_t featureID) override;

	///
	virtual const void* getFeature(size_t featureID) const override;

	virtual void loadFromConfig(const Json& config) {}

	///
	virtual void saveToConfig(Json& config) const {}


	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;
};

}