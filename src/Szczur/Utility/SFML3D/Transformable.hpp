#pragma once

#include "Transform.hpp"

namespace sf3d {
	class Transformable {
	public:
		void setPosition(const glm::vec3& position);
		void move(const glm::vec3& offset);
		const glm::vec3& getPosition() const;

		void setRotation(const glm::vec3& direction);
		void rotate(const glm::vec3& direction);
		const glm::vec3& getRotation() const;

		void setScale(const glm::vec3& value);
		void scale(const glm::vec3& value);
		const glm::vec3& getScale() const;

		void setOrigin(const glm::vec3& position);
		const glm::vec3& getOrigin() const;

		Transform getTransform() const;

	private:
		glm::vec3 _position{0.f, 0.f, 0.f};

		glm::vec3 _rotation{0.f, 0.f, 0.f};

		glm::vec3 _scale{1.f, 1.f, 1.f};

		glm::vec3 _origin{0.f, 0.f, 0.f};
	};
}