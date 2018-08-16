#include "Shape.hpp"

#include <glm/vec4.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf3d
{

/* Properties */
std::size_t Shape::getVerticesCount() const
{
	return this->vertices.getSize();
}

glm::vec4 Shape::getColor() const
{
	// @warn Return only first vertex color
	return this->vertices[0].color;
}
void Shape::setColor(const glm::vec4& color)
{
	for (std::size_t i = 0; i < this->vertices.getSize(); ++i) {
		this->vertices[i].color = color;
	}
}

void Shape::setColorSFML(const sf::Color& sfcolor)
{
	this->setColor({
		static_cast<float>(sfcolor.r) / 255.f,
		static_cast<float>(sfcolor.g) / 255.f,
		static_cast<float>(sfcolor.b) / 255.f,
		static_cast<float>(sfcolor.a) / 255.f
	});
}



/* Operators */
Shape::Shape(std::size_t vertexSize, sf3d::PrimitiveType primitiveType) 
:	vertices(vertexSize, primitiveType)
{
	;
}



/* Methods */
void Shape::draw(RenderTarget& target, RenderStates states) const
{ 
	states.transform *= getTransform(); 
	target.draw(this->vertices, states); 
} 

}
