#pragma once

/** @file SpriteComponent.hpp
 ** @description Header file with sprite component class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <array>

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "Szczur/Modules/World/Componable.hpp"
#include "Szczur/Modules/World/BaseObject.hpp"
#include "Szczur/Modules/World/DisplayData/SpriteDisplayData.hpp"

namespace rat
{

/** @class SpriteComponent
 ** @implements sf3d::Drawable
 ** @inheirts Component<BaseObject>
 ** @description Simplest, sprite component for displaying object.
 **/
class SpriteComponent : public Componable::Component<BaseObject>, public sf3d::Drawable
{
	/* Fields */
protected:
	SpriteDisplayData* displayData {nullptr}; // @todo . shared 



	/* Properties */
protected:
	/** @property Texture
	 ** @description Texture used to draw the object.
	 ** @access const get
	 **/
	const sf3d::Texture& getTexture() const;

	/** @property Vertices
	 ** @description Vertices used to draw the texture on the target.
	 ** @access const get
	 **/
	const sf3d::VertexArray& getVertices() const;



	/* Operators */
public:
	SpriteComponent();
	SpriteComponent(SpriteDisplayData* displayData);
	SpriteComponent(SpriteComponent&& other);



	/* Methods */
protected:
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override;
};

}
