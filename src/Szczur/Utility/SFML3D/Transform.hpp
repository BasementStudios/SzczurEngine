#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sf3d {
	class Transform {
	public:
		Transform();

		inline void translate(float x, float y, float z);
		void translate(const glm::vec3& offset);

		inline void rotate(float angle, float x, float y, float z);
		void rotate(float angle, const glm::vec3& direction);

		inline void scale(float x, float y, float z);
		void scale(const glm::vec3& value);

		void combine(const Transform& trans);

		void operator*=(const Transform& trans);

		void operator*=(const glm::vec3& value);

		glm::mat4& getMatrix();
	private:
		glm::mat4 _mat;
	};
}