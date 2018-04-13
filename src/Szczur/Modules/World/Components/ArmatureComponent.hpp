#pragma once

/** @file ArmatureComponent.hpp
 ** @description Header file with armature component class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

namespace dragonBones {
	class Animation;
	class SFMLArmatureDisplay;
}

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
 ** @implements sf3d::Drawable
 ** @inheirts Component<BaseObject>
 ** @description DragonBones armature component for displaying object.
 **/
class ArmatureComponent : public Componable::Component<BaseObject>, public sf3d::Drawable
{
	/* Fields */
protected:
	ArmatureDisplayData*                displayData {nullptr}; // @todo . shared 
	
	dragonBones::SFMLArmatureDisplay*   pose        {nullptr}; // @todo . Own armature system instead of static factory-pattern...



	/* Properties */
public:
	/** @property Animation 
	 ** @description Provides access to DragonBones Animation object of current pose.
	 ** @access pointer get
	 **/
	dragonBones::Animation* getAnimation();



	/* Operators */
public:
	ArmatureComponent();
	ArmatureComponent(ArmatureDisplayData* displayData, const std::string& animation = "");
	ArmatureComponent(ArmatureComponent&& other);



	/* Methods */
protected:
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;
};

}
