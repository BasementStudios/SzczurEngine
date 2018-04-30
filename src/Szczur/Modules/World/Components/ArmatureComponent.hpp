#pragma once

/** @file ArmatureComponent.hpp
 ** @description Header file with armature component class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @music Taconafide - SOMA
 **/

#include <string>
#include <memory> // unique_ptr
#include <vector>

namespace dragonBones {
	class Animation;
	class Armature;
	class SF3DDisplay;
}
#include <dragonBones/armature/IArmatureProxy.h>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
namespace sf3d {
	class Texture;
	class RenderTarget;
	class RenderStates;
}

#include "Szczur/Modules/World/Componable.hpp"
#include "Szczur/Modules/World/BaseObject.hpp"
namespace rat {
	class ArmatureDisplayData;
}

namespace rat
{

/** @class ArmatureComponent
 ** @inheirts Component<BaseObject>
 ** @implements sf3d::Drawable, dragonBones::IArmatureProxy
 ** @description DragonBones armature component for displaying object.
 **/
class ArmatureComponent
	: public Component<BaseObject>, public sf3d::Drawable, public dragonBones::IArmatureProxy
{
	/* Fields */
protected:
	ArmatureDisplayData* displayData {nullptr}; // @todo . shared 
	
	dragonBones::Armature* armature;

	std::vector<std::unique_ptr<dragonBones::SF3DDisplay>> displays;



	/* Properties */
public:
	/** @property Armature
	 ** @description Provides access to DragonBones Armature object of current pose.
	 ** @access pointer get
	 ** @override dragonBones::IArmatureProxy
	 **/
	dragonBones::Armature* getArmature() const override;
	/** @property Animation 
	 ** @description Provides access to DragonBones Animation object of current pose.
	 ** @access pointer get
	 ** @override dragonBones::IArmatureProxy
	 **/
	dragonBones::Animation* getAnimation() const override;



	/* Operators */
public:
	// Constructors 
	ArmatureComponent();
	ArmatureComponent(ArmatureDisplayData* displayData, const std::string& animation = "");
	
	// Destructor
	~ArmatureComponent();

	// Movable
	ArmatureComponent(ArmatureComponent&& other);
	// @todo , move operator = 



	/* Methods */
protected:
	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;
	// @todo void update(float deltaTime) override { this->getArmature()->advanceTime(deltaTime); }

	// Satisfy dragonBones::IArmatureProxy interface
public: // @todo ? protected & friend Armature?
	void dbInit(dragonBones::Armature* armature) override;
	void dbClear() override;
	void dbUpdate() override;
	void dispose(bool disposeProxy) override;
};

}
