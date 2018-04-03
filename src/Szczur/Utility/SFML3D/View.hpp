#pragma once


#include "Transform.hpp"
#include <glm/glm.hpp>

namespace sf3d {
	class View {
	public:
		View(float factor, const glm::vec3& center = {0.f, 0.f, 0.f});

		void move(const glm::vec3& offset);
		void setCenter(const glm::vec3& offset);

		void rotate(const glm::vec3& direction);
		void setRotation(const glm::vec3& direction);

		Transform getTransform() const;
	private:
		float _factor;
		glm::vec3 _center;
		glm::vec3 _rotation{0.f, 0.f, 0.f};
	};
}