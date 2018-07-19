#pragma once

#include <glm/glm.hpp>

namespace sf3d {
	class Linear {
	public:
		glm::vec3 _position;
		glm::vec3 _direction;
		Linear(const glm::vec3& position, const glm::vec3& direction);

		bool contains(const glm::vec3& pos, const glm::vec3& size) const;

		glm::vec3 getProjectionY(float height);
		glm::vec3 getProjectionZ(float depth);
		
		glm::vec3 getCameraProjectionY(float height);
		glm::vec3 getCameraProjectionZ(float depth);
	};
}
