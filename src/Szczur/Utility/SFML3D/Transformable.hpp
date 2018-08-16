#pragma once

#include <glm/vec3.hpp>

namespace sf3d {
	class Transform;
}

namespace sf3d
{

class Transformable {
	/* Variables */
private:
	glm::vec3 _position	{0.f, 0.f, 0.f};

	glm::vec3 _rotation	{0.f, 0.f, 0.f};

	glm::vec3 _scale 	{1.f, 1.f, 1.f};

	glm::vec3 _origin 	{0.f, 0.f, 0.f};



	/* Properties */
public:
	/// Position of the transformable
	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	/// Rotation on each axis
	void setRotation(const glm::vec3& direction);
	const glm::vec3& getRotation() const;
	
	/// Scale by each axis
	void setScale(const glm::vec3& value);
	const glm::vec3& getScale() const;
	
	/// Relative origin position to rotate from
	void setOrigin(const glm::vec3& position);
	const glm::vec3& getOrigin() const;



	/* Operators */
public:
	Transformable(
		const glm::vec3& position = {}, 
		const glm::vec3& rotation = {},
		const glm::vec3& scale = {1.f, 1.f, 1.f},
		const glm::vec3& origin = {0.f, 0.f, 0.f}
	);


	
	/* Methods */
public:
	/// Changes position by offset @todo ? should be named `translate`, since `move` also should check movement outcomes (collisions, etc.)
	void move(const glm::vec3& offset);

	/// Rotates on each axis by offsets
	void rotate(const glm::vec3& offset);

	/// Scales by each axis by value
	void scale(const glm::vec3& value);
	//void scale(float value); // @todo ,

	/// Returns calculated Transform (matrix).
	Transform getTransform() const;
};

}