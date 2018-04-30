#include "ArmatureComponent.hpp"

/** @file ArmatureComponent.cpp
 ** @description Implementation file for armature component class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @music KęKę - Trzecie Rzeczy
 **/

#include <string>
#include <memory> // unique_ptr
#include <vector>
#include <utility> // pair

#include <SFML/Graphics/Color.hpp>

#include <dragonBones/armature/Armature.h>
#include <dragonBones/animation/Animation.h>
#include <dragonBones/armature/Bone.h>		
#include "Szczur/Modules/DragonBones/SF3DSlot.hpp"
#include "Szczur/Modules/DragonBones/SF3DDisplay.hpp"
#include <dragonBones/model/ArmatureData.h> // ArmatureData, BoneData, SlotData
#include <dragonBones/model/AnimationData.h>
#include <dragonBones/model/DisplayData.h> // DisplayData, ImageDisplayData, MeshDisplayData, ArmatureDisplayData
#include <dragonBones/model/SkinData.h>
#include <dragonBones/core/DragonBones.h> // DisplayType

namespace sf3d {
	class Texture;
	class VertexArray;
	class RenderTarget;
	class RenderStates;
}

#include "Szczur/Modules/World/DisplayData/ArmatureDisplayData.hpp"

namespace rat
{

/* Properties */
/// Armature
dragonBones::Armature* ArmatureComponent::getArmature() const
{
	return this->armature;
}
/// Animation
dragonBones::Animation* ArmatureComponent::getAnimation() const
{
	return this->armature->getAnimation();
}



/* Operators */
// Constructors
ArmatureComponent::ArmatureComponent()
{};
ArmatureComponent::ArmatureComponent(ArmatureDisplayData* displayData, const std::string& animation)
:
	displayData(displayData)
{
	using dragonBones::Armature;
	using dragonBones::ArmatureData;
	using dragonBones::Bone;
	using dragonBones::BoneData;
	using dragonBones::SF3DSlot;
	using dragonBones::SlotData;
	using dragonBones::SF3DDisplay;
	using dragonBones::DragonBonesData;
	using dragonBones::SkinData;
	
	const DragonBonesData* skeletonData = this->displayData->skeletonData;
	const ArmatureData* armatureData = skeletonData->getArmature(this->displayData->name);
	const SkinData* skinData = skeletonData->defaultSkin; // @todo , skin support (or alternative, since DB skins are poor, per armature (instead of slot and unaccessable from the editor)
	
	// Init Armature
	LOG_INFO("{*", this, "\"", this->name, "} ", "Initializing armature: `", armatureData->name, "`...");
	this->armature = dragonBones::BaseObject::borrowObject<Armature>();
	this->armature->init(armatureData, this, this, nullptr); // @warn _dragonBones == nullptr, _armatureData must be nullptr on Armature::dispose()

	// Init bones
	LOG_INFO("{*", this, "\"", this->name, "} ", "Initializing bones...");
	for (const BoneData* boneData : armatureData->sortedBones) {
		Bone* bone = dragonBones::BaseObject::borrowObject<Bone>();
		bone->init(boneData, this->armature);
	}

	// Init slots
	for (const SlotData* slotData : armatureData->sortedSlots) {
		LOG_INFO("{*", this, "\"", this->name, "} ", "Initializing slot: `", slotData->name, "`...");
		
		// Create and init
		SF3DSlot* slot = dragonBones::BaseObject::borrowObject<SF3DSlot>();
		SF3DDisplay* display = new SF3DDisplay(); // @todo . delete
		slot->init(slotData, this, display, display);

		// Setup display datas
		const std::vector<dragonBones::DisplayData>& displayDatas = skinData->displays[slotData->name];
		slot->setRawDisplayDatas(&displayDatas);

		// Init displayers 
		std::vector<std::pair<void*, dragonBones::DisplayType>> displayList;
		for (const dragonBones::DisplayData& displayDataRef : displayDatas) {
			const dragonBones::DisplayData* displayData = &displayDataRef;
			if (displayData != nullptr) {
				switch (displayData->type) {
					case dragonBones::DisplayType::Image:
					{
						const dragonBones::ImageDisplayData* imageDisplayData = static_cast<const dragonBones::ImageDisplayData*>(displayData);
						const_cast<dragonBones::ImageDisplayData*>(imageDisplayData)->texture = this->displayData->getTextureData(displayData->path);
						displayList.emplace_back(slot->_rawDisplay, dragonBones::DisplayType::Image);
						break;
					}

					case dragonBones::DisplayType::Mesh:
					{
						const dragonBones::MeshDisplayData* meshDisplayData = static_cast<const dragonBones::MeshDisplayData*>(displayData);
						const_cast<dragonBones::MeshDisplayData*>(meshDisplayData)->texture = this->displayData->getTextureData(displayData->path);
						displayList.emplace_back(slot->_rawDisplay, dragonBones::DisplayType::Mesh);
						break;
					}

					case dragonBones::DisplayType::Armature:
					{
						// @todo . ;(
						LOG_ERROR("{*", this, "\"", this->name, "} ", "Sorry kurwa ale nie da rady na razie ;("); 
					}
					
					default:
						break;
				}
			}
			else {
				displayList.emplace_back(nullptr, dragonBones::DisplayType::Image);
			}
		}
		slot->_setDisplayList(displayList);

		// Set display index
		slot->_setDisplayIndex(slotData->displayIndex, true);
	}

	// Do first update of armature
	this->armature->invalidUpdate("", true);
    this->armature->advanceTime(0.0f);

	// Set starting animation
	if (!animation.empty()) {
		this->getAnimation()->play(animation); // @todo , whole config
	}

	LOG_INFO("{*", this, "\"", this->name, "} ", "Initialized");
};

// Destructor
ArmatureComponent::~ArmatureComponent()
{
	if (this->armature) {
		delete this->armature;
	}
}

// Movable
ArmatureComponent::ArmatureComponent(ArmatureComponent&& other)
{
	this->armature = other.armature;
	other.armature = nullptr;
	this->displayData = other.displayData; // @todo , shared (remember std::move!)
	other.displayData = nullptr;
	this->displays = std::move(other.displays);
};
// @todo , move operator = 



/* Methods */
/// draw
void ArmatureComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	// @todo . usunąć SF3DArmatureDisplay, używać bezpośrednio naszego Transforma
	states.transform *= this->getTransform();
	//target.draw(this->armature, states); bones!
}

// Satisfy dragonBones::IArmatureProxy interface
void ArmatureComponent::dbInit(dragonBones::Armature* armature)
{
	this->armature = armature;
}
void ArmatureComponent::dbClear()
{
	this->armature = nullptr;
	//this->BaseObject::dispose(); // @todo . This is invaild state of ArmatureComponent.
}
void ArmatureComponent::dbUpdate()
{
	; // @info Should be never used; instead use `update(float deltaTime)`
}
void ArmatureComponent::dispose(bool disposeProxy)
{
	LOG_WARNING("dispose?"); // @test
	if (this->armature) {
		delete this->armature;
		this->armature = nullptr;
	}
}

}
