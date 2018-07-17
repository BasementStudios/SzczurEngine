#pragma once

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "VertexArray.hpp"

namespace sf3d
{

class Shape : public Drawable, public Transformable
{
public:
	/* Variables */
	VertexArray _vertices;
	


	/* Properties */
	virtual glm::vec3 getPointPosition(std::size_t index) const = 0;
	virtual glm::vec4 getPointColor(std::size_t index) const = 0;
	virtual glm::vec2 getPointCoord(std::size_t index) const = 0;

	std::size_t getVerticesCount() const;



	/* Operators */
protected:
	Shape(std::size_t vertexSize = 4u);



	/* Methods */
public:
	virtual void draw(RenderTarget& target, RenderStates states) const override;

protected:
	void _update();
};

}
