#pragma once

#include <glm/glm.hpp>

namespace sf3d {
	struct Vertex {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec2 texCoord = { 0.0f, 0.0f };
	};
}
