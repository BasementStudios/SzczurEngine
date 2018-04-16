#pragma once

#include "Transform.hpp"

namespace sf3d {

class Transformable {
	/* Fields */
private:
	glm::vec3 _position {0.f, 0.f, 0.f};

	glm::vec3 _rotation {0.f, 0.f, 0.f};

	glm::vec3 _scale    {1.f, 1.f, 1.f};

	glm::vec3 _origin   {0.f, 0.f, 0.f};
	


	/* Properties */
public:
	/// Position
	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	/// Rotation
	void setRotation(const glm::vec3& direction);
	const glm::vec3& getRotation() const;
	
	/// Scale
	void setScale(const glm::vec3& value);
	const glm::vec3& getScale() const;
	
	/// Origin
	void setOrigin(const glm::vec3& position);
	const glm::vec3& getOrigin() const;



	/* Methods */
public:
	// Transform
	Transform getTransform() const;

	// Position
	void translate(const glm::vec3& offset);

	// Rotation
	void rotate(const glm::vec3& direction);

	// Scale
	void scale(const glm::vec3& value);

	// @todo . dodać podobne jak w Transform ;f
};

}