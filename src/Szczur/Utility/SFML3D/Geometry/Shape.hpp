#pragma once

#include <glm/vec4.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../Drawable.hpp"
#include "../Transformable.hpp"
#include "../VertexArray.hpp"

namespace sf3d
{

class Shape : public Drawable, public Transformable
{
protected:
	/* Variables */
	VertexArray vertices;
	


	/* Properties */
	std::size_t getVerticesCount() const;

	/// Color used to display whole shape 
	glm::vec4 getColor() const; // @warn Return only first vertex color
	void setColor(const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

	[[deprecated("SF3D should operate on its own color class in the future, but at now it is `glm::vec4`.")]]
	void setColorSFML(const sf::Color& color = sf::Color::Transparent);



	/* Operators */
protected:
	Shape(std::size_t vertexSize = 4u, sf3d::PrimitiveType primitiveType = sf3d::PrimitiveType::TriangleFan);



	/* Methods */
public:
	virtual void draw(RenderTarget& target, RenderStates states) const override;
};

}
