#include "ArmatureComponent.hpp"
 
#include <iostream>
#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"
 
namespace rat
{
    
    ArmatureComponent::ArmatureComponent(ArmatureDisplayData* armatureDisplayData) :
    Component(typeID<ArmatureComponent>(), "ArmatureComponent", typeID<sf3d::Drawable>())
    {
        setArmatureDisplayData(armatureDisplayData);
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
    
    std::unique_ptr<Component> ArmatureComponent::copy() const {
        return std::make_unique<ArmatureComponent>(*this);
    }
    
    const void* ArmatureComponent::getFeature(size_t featureID) const {
        if (featureID == typeID<sf3d::Drawable>())
            return static_cast<const sf3d::Drawable*>(this);
        
        return nullptr;
    }
    
    void* ArmatureComponent::getFeature(size_t featureID) {
        if (featureID == typeID<sf3d::Drawable>())
            return static_cast<sf3d::Drawable*>(this);
        
        return nullptr;
    }
    
    void ArmatureComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
    {
        if(_armature)
            _armature->draw(target, states);
    }
 
}