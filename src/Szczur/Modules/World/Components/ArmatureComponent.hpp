#pragma once

#include <memory> // unique_ptr

namespace dragonBones {
    class SF3DArmatureDisplay;
}

namespace rat
{
    class Entity;
    class ArmatureDisplayData;
}
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Modules/World/Component.hpp"

namespace rat
{

class ArmatureComponent : public Component, public sf3d::Drawable
{
private:

    dragonBones::SF3DArmatureDisplay* _armature = nullptr;
    ArmatureDisplayData* _armatureDisplayData = nullptr;

public:

    ArmatureComponent(Entity* parent);

    ~ArmatureComponent();

    void setArmatureDisplayData(ArmatureDisplayData* armatureDisplayData);

    ArmatureDisplayData* getArmatureDisplayData();

    const ArmatureDisplayData* getArmatureDisplayData() const;

    dragonBones::SF3DArmatureDisplay* getArmature();

    virtual std::unique_ptr<Component> copy(Entity* newParent) const;

    virtual void* getFeature(Component::Feature_e feature) override;

    virtual const void* getFeature(Component::Feature_e feature) const override;

    virtual void loadFromConfig(nlohmann::json& config) {}

    virtual void saveToConfig(nlohmann::json& config) const {}

    void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;
};

}
