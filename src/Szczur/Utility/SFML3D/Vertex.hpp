#pragma once

#include <glm/glm.hpp>
namespace sf3d {
	class Vertex {
	public:
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
	};
}