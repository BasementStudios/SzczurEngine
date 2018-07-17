#pragma once

#include "Transformable.hpp"
#include "Drawable.hpp"
#include "VertexArray.hpp"

namespace sf3d
{
	class RenderTarget;
	class Texture;
	class RenderStates;
}

namespace sf3d
{

class Sprite : public Transformable, public Drawable
{
	/* Variables */
private:
	const Texture* _texture {nullptr}; // @todo ?
	VertexArray _vertices {4u, sf3d::PrimitiveType::TriangleFan};



	/* Properties */
public:
	/// Set new texture for sprite
	void setTexture(const Texture& texture);



	/* Operators */
public:
	Sprite();

	/// Constructs sprite with given texture
	Sprite(const Texture& texture);


	
	/* Methods */
	virtual void draw(RenderTarget& target, RenderStates states) const override;
};

}
