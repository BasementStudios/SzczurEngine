#include "Shape.hpp"

namespace sf3d
{

/* Properties */
std::size_t Shape::getVerticesCount() const
{
	return _vertices.getSize();
}



/* Operators */
Shape::Shape(std::size_t vertexSize) 
:	_vertices(vertexSize, sf3d::PrimitiveType::TriangleFan)
{
	;
}



/* Methods */
void Shape::draw(RenderTarget & target, RenderStates states) const
{ 
	states.transform *= getTransform(); 
	target.draw(_vertices, states); 
} 

void Shape::_update()
{
	for (std::size_t i = 0; i < _vertices.getSize(); ++i) {
		_vertices[i].position = this->getPointPosition(i);
		_vertices[i].color    = this->getPointColor(i);
		_vertices[i].texCoord = this->getPointCoord(i);
	}
}

}
