#include "ArmatureComponent.hpp"

/** @file ArmatureComponent.cpp
 ** @description Implementation file for armature component class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/Graphics/Color.hpp>
#include <dragonBones/SFMLArmatureDisplay.h>
#include <dragonBones/animation/Animation.h>

#include "Szczur/Utility/SFML3D/RenderTexture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/Vertex.hpp"
namespace sf3d {
	class Texture;
	class VertexArray;
	class RenderTarget;
	class RenderStates;
}

#include "Szczur/Modules/World/DisplayData/ArmatureDisplayData.hpp"

namespace rat
{

// @todo ! zrobić normalnie to a nie... =,=
static sf3d::RenderTexture tmpRenTex(
	"Assets/Shaders/default/vertex.shader", 
	"Assets/Shaders/default/fragment.shader",
	{1000.f, 1000.f}
);
static sf3d::VertexArray tmpVertArr {4};



/* Properties */
/// Animation
dragonBones::Animation* ArmatureComponent::getAnimation()
{
	return this->pose->getAnimation();
}



/* Operators */
/// Constructors
ArmatureComponent::ArmatureComponent()
{};
ArmatureComponent::ArmatureComponent(ArmatureDisplayData* displayData, const std::string& animation)
:
	displayData(displayData)
{
	// @todo ! zrobić normalnie to a nie... =,=
	// xd i tak nie mialem gdzie tego dac, wiec here na szybko
	auto& size = tmpRenTex.getSize();
	tmpVertArr.set(0u, {
		{0.f, 0.f, 0.f},
		{1.f, 1.f, 1.f},
		{0.f, 0.f}
	});
	tmpVertArr.set(1u, {
		{(float)size.x, 0.f, 0.f},
		{1.f, 1.f, 1.f},
		{1.f, 0.f}
	});
	tmpVertArr.set(2u, {
		{(float)size.x, -(float)size.y, 0.f},
		{1.f, 1.f, 1.f},
		{1.f, 1.f}
	});
	tmpVertArr.set(3u, {
		{0.f, -(float)size.y, 0.f},
		{1.f, 1.f, 1.f},
		{0.f, 1.f}
	});
	//;
	
	this->pose = displayData->getFactory().buildArmatureDisplay(displayData->name);

	if (!animation.empty()) {
		this->getAnimation()->play(animation); // @todo , whole config
	}
};

/// Destructor
ArmatureComponent::~ArmatureComponent()
{
	if (this->pose) {
		delete this->pose;
	}
}

/// Movable
ArmatureComponent::ArmatureComponent(ArmatureComponent&& other)
{
	this->pose = other.pose;
	other.pose = nullptr;
};



/* Methods */
/// draw
void ArmatureComponent::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	// @todo ! zrobić normalnie to a nie... =,=
	tmpRenTex.clear(sf::Color(0u, 0u, 0u, 0u));
	tmpRenTex.draw(this->pose);
	
	states.transform *= this->getTransform();
	states.texture = &(tmpRenTex);
	target.draw(tmpVertArr, states);
}

}
