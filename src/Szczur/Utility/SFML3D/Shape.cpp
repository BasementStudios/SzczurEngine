#include "Shape.hpp"

namespace sf3d {
	Shape::Shape(size_t vertexSize) :
		_vertices(vertexSize) {
		for(size_t i = 0; i < vertexSize; ++i) {
			_vertices.setPosition(i, getPointPosition(i));
			_vertices.setColor(i, getPointColor(i));
			_vertices.setTexCoord(i, getPointCoord(i));
		}
	}

	void Shape::draw(RenderTarget & target, RenderStates states) const {
		states.transform *= getTransform();
		target.draw(_vertices, states);
	}

	glm::vec3 Shape::getPointPosition(size_t index) const {
		switch(index) {
			case 0:
			return {0.f, 0.f, 0.f};
			break;
			case 1:
			return {768.f, 0.f, 0.f};
			break;
			case 2:
			return {768.f, -512.f, 0.f};
			break;
			case 3:
			return {0.f, -512.f, 0.f};
			break;
		}
	}

	glm::vec3 Shape::getPointColor(size_t index) const {
		switch(index) {
			case 0:
			return {1.f, 0.f, 0.f};
			break;
			case 1:
			return {0.f, 1.f, 0.f};
			break;
			case 2:
			return {0.f, 0.f, 1.f};
			break;
			case 3:
			return {1.f, 0.f, 1.f};
			break;
		}
	}

	glm::vec2 Shape::getPointCoord(size_t index) const {
		switch(index) {
			case 0:
			return {0.f, 0.f};
			break;
			case 1:
			return {1.f, 0.f};
			break;
			case 2:
			return {1.f, 1.f};
			break;
			case 3:
			return {0.f, 1.f};
			break;
		}
	}
}