#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace sf3d {
	class Transformable;
}

namespace sf3d
{

/// Calculated class of Transformable.
class Transform
{
	/* Variables */
private:
	glm::mat4 matrix;



	/* Properties */
public:
	glm::mat4& getMatrix();
	const glm::mat4& getMatrix() const;



	/* Operators */
public:
	Transform();
	Transform(const glm::mat4& matrix);
	Transform(const Transformable& matrix);

	// Combining via multipy operator
	void operator *= (const Transform& other);
	void operator *= (const glm::vec3 value);



	/* Methods */
public:
	void translate(const glm::vec3 offset);
	void translate(const float x, const float y, const float z);

	void rotate(const float angle, const glm::vec3 direction);
	void rotate(const float angle, const float x, const float y, const float z);
	void rotate(const glm::vec3 rotation);
	void rotate(const float x, const float y, const float z);

	void scale(const glm::vec3 value);
	void scale(const float x, const float y, const float z);

	void combine(const Transform& trans);
};

}
