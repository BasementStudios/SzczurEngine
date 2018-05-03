#pragma once

#include <glm/glm.hpp>

namespace sf3d {
	class Linear {
	public:
		glm::vec3 _position;
		glm::vec3 _direction;
		Linear(const glm::vec3& position, const glm::vec3& direction);

		bool contains(glm::vec3 pos, glm::vec3 size) const;
	};
}
