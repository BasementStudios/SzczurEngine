#pragma once

/** @file Camera.hpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace sf3d
{

/// Describes camera that provide view matrix.
class Camera
{
	/* Variables */
private:
	glm::vec3 position;
	glm::vec3 rotation;

	// Precalculate directional vectors
	glm::vec3 upVector;
	glm::vec3 rightVector;
	glm::vec3 frontVector;

	// Buffering view matrix product, to avoid recalculations while not manipulating camera.
	glm::mat4 viewMatrix;

	

	/* Properties */
public:
	/// Position in space
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition() const;

	/// Rotation in each axis
	glm::vec3 getRotation() const;
	void setRotation(glm::vec3 rotation);

	/// Normalized front vector which point direction where camera look
	glm::vec3 getFrontVector() const;
	void setFrontVector(glm::vec3 vector);

	/// Normalized up vector which point direction where camera
	glm::vec3 getUpVector() const;
	void setUpVector(glm::vec3 vector);

	/// Normalized right vector //@todo , opis
	glm::vec3 getRightVector() const;
	void setRightVector(glm::vec3 vector);

	// @todo ? getTransform

	/// View matrix used to render camera vision
	glm::mat4 getViewMatrix() const;
	


	/* Operators */
public:
	Camera(
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f)
	);



	/* Methods */
public:
	/// Changes position by given offset
	void move(glm::vec3 offset);

	/// Changes rotatio by given offsets for each axis
	void rotate(glm::vec3 rotation);

	/// Rotates camera to given position in space
	void faceTo(glm::vec3 position);

	/// 
	void bartek(glm::vec3 direction);

private:
	void updateViewMatrix();

	void updateDirectionalVectors();
};

}
