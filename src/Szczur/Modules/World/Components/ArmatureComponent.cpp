#include "ArmatureComponent.hpp"

#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"

#include "../Entity.hpp"

namespace rat
{

    ArmatureComponent::ArmatureComponent(Entity* parent) :
        Component(parent, fnv1a_64("ArmatureComponent"), "ArmatureComponent", Component::Drawable)
    {

    }

    ArmatureComponent::~ArmatureComponent()
    {
        if(_armature)
            delete _armature;
    }

    void ArmatureComponent::setArmatureDisplayData(ArmatureDisplayData* armatureDisplayData)
    {
        _armatureDisplayData = armatureDisplayData;
        if(armatureDisplayData) {
            auto dbFactory = dragonBones::SF3DFactory::get();
            if(_armature)
            delete _armature;
            _armature = dbFactory->buildArmatureDisplay(armatureDisplayData->getName());
        }
        else if(_armature) {
            delete _armature;
            _armature = nullptr;
        }
    }

    ArmatureDisplayData* ArmatureComponent::getArmatureDisplayData() {
        return _armatureDisplayData;
    }

    const ArmatureDisplayData* ArmatureComponent::getArmatureDisplayData() const {
        return _armatureDisplayData;
    }

    dragonBones::SF3DArmatureDisplay* ArmatureComponent::getArmature() {
        return _armature;
    }

    std::unique_ptr<Component> ArmatureComponent::copy(Entity* newParent) const {
        auto ptr = std::make_unique<ArmatureComponent>(*this);

        ptr->setEntity(newParent);

        return ptr;
    }

    void* ArmatureComponent::getFeature(Component::Feature_e feature) {
        if (feature == Component::Drawable) return static_cast<sf3d::Drawable*>(this);

        return nullptr;
    }

    const void* ArmatureComponent::getFeature(Component::Feature_e feature) const {
        if (feature == Component::Drawable) return static_cast<const sf3d::Drawable*>(this);

        return nullptr;
    }

    void ArmatureComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
    {
        if(_armature) {
            states.transform *= getEntity()->getTransform();
            _armature->draw(target, states);
        }
    }

}
