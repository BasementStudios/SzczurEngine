#pragma once


#include "Transform.hpp"
#include <glm/glm.hpp>

namespace sf3d {
	class View {
	public:
		View(float factor=0.f, const glm::vec3& center = {0.f, 0.f, 0.f});

		void move(const glm::vec3& offset);
		void setCenter(const glm::vec3& offset);

		const glm::vec3& getCenter() const;

		void rotate(const glm::vec3& direction);
		void setRotation(const glm::vec3& direction);

		const glm::vec3& getRotation() const;

		Transform getTransform() const;

		void create(float factor, const glm::vec3& center);
	private:
		float _factor;
		glm::vec3 _center;
		glm::vec3 _rotation{0.f, 0.f, 0.f};
	};
}