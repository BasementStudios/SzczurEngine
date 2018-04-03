#pragma once

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "VertexArray.hpp"

namespace sf3d {
	class Shape : public Drawable, public Transformable {
	public:
		Shape(size_t vertexSize = 4u);

		virtual void draw(RenderTarget& target, RenderStates states) const override;

		virtual glm::vec3 getPointPosition(size_t index) const;
		virtual glm::vec3 getPointColor(size_t index) const;
		virtual glm::vec2 getPointCoord(size_t index) const;
	private:
		VertexArray _vertices;
	};
}