#include "Camera.hpp"

/** @file Camera.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp> // radians, cos, sin

namespace sf3d
{

/* Properties */
// Position
void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
	this->updateViewMatrix();
}
glm::vec3 Camera::getPosition() const
{
	return this->position;
}

// Rotation
void Camera::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
	this->updateDirectionalVectors();
	this->updateViewMatrix();
}
glm::vec3 Camera::getRotation() const
{
	return this->rotation;
}

// Front vector
void Camera::setFrontVector(glm::vec3 frontVector)
{
	LOG_ERROR("Not implemented"); // @todo 
	this->frontVector = frontVector;
	this->updateViewMatrix();
}
glm::vec3 Camera::getFrontVector() const
{
	return this->frontVector;
}

// Right vector
void Camera::setRightVector(glm::vec3 rightVector)
{
	LOG_ERROR("Not implemented"); // @todo 
	this->rightVector = rightVector;
	this->updateViewMatrix();
}
glm::vec3 Camera::getRightVector() const
{
	return this->rightVector;
}

// Up vector
void Camera::setUpVector(glm::vec3 upVector)
{
	LOG_ERROR("Not implemented"); // @todo 
	this->upVector = upVector;
	this->updateViewMatrix();
}
glm::vec3 Camera::getUpVector() const
{
	return this->upVector;
}

// ViewMatrix
glm::mat4 Camera::getViewMatrix() const
{
	return this->viewMatrix; 
}



/* Operators */
Camera::Camera(glm::vec3 position, glm::vec3 rotation)
:	position(position), rotation(rotation)
{
	this->updateDirectionalVectors();
	this->updateViewMatrix();
}



/* Methods */
void Camera::updateViewMatrix()
{
	// this->viewMatrix = glm::mat4(1.f);
	// this->viewMatrix = glm::rotate(this->viewMatrix, glm::radians(this->rotation.x), {1.f, 0.f, 0.f});
	// this->viewMatrix = glm::rotate(this->viewMatrix, glm::radians(this->rotation.y), {0.f, 1.f, 0.f});
	// this->viewMatrix = glm::rotate(this->viewMatrix, glm::radians(this->rotation.z), {0.f, 0.f, 1.f});
	// this->viewMatrix = glm::translate(this->viewMatrix, -this->position);

	// this->viewMatrix = ( // @todo ! ! !
	// 	glm::yawPitchRoll(
	// 		glm::radians(this->rotation.y), // Yaw
	// 		glm::radians(this->rotation.x), // Pitch
	// 		glm::radians(this->rotation.z)	// Roll
	// 	) *
	// 	glm::translate(-this->position) 
	// );

	glm::vec3 Z = -this->frontVector;
	glm::vec3 X = glm::cross(this->upVector, Z);
	glm::vec3 Y = glm::cross(Z, X);

	this->viewMatrix[0][0] = X.x;
	this->viewMatrix[1][0] = X.y;
	this->viewMatrix[2][0] = X.z;
	this->viewMatrix[3][0] = glm::dot(-X, this->position);
	this->viewMatrix[0][1] = Y.x;
	this->viewMatrix[1][1] = Y.y;
	this->viewMatrix[2][1] = Y.z;
	this->viewMatrix[3][1] = glm::dot(-Y, this->position);
	this->viewMatrix[0][2] = Z.x;
	this->viewMatrix[1][2] = Z.y;
	this->viewMatrix[2][2] = Z.z;
	this->viewMatrix[3][2] = glm::dot(-Z, this->position);
	this->viewMatrix[0][3] = 0.f;
	this->viewMatrix[1][3] = 0.f;
	this->viewMatrix[2][3] = 0.f;
	this->viewMatrix[3][3] = 1.f;
}

void Camera::updateDirectionalVectors()
{
	const float pitch 	= glm::radians(this->rotation.x);
	const float yaw 	= glm::radians(this->rotation.y);
	const float roll 	= glm::radians(this->rotation.z);

	// Front
	const float cosYaw = glm::cos(yaw);
	
	// this->frontVector.x = -cos(pitch) * sin(yaw); // -0 -0
    // this->frontVector.y = sin(pitch);
    // this->frontVector.z = -cos(pitch) * cos(yaw);
	
	// this->frontVector.x = cos(pitch) * cos(yaw); // 0 0 +X
    // this->frontVector.y = sin(pitch);
    // this->frontVector.z = cos(pitch) * sin(yaw);

	// this->frontVector.x = cos(pitch) * sin(yaw); // 0 0 +Z
    // this->frontVector.y = sin(pitch);
    // this->frontVector.z = cos(pitch) * cos(yaw);

	this->frontVector.x = -(cos(pitch) * sin(yaw)); // 0 0 -Z
    this->frontVector.y = sin(pitch);
    this->frontVector.z = -(cos(pitch) * cos(yaw));
	
	this->frontVector = glm::normalize(this->frontVector);

	// Right
	this->rightVector = glm::normalize(glm::cross(this->frontVector, 
		glm::vec3{0.f, 1.f, 0.f}
		//glm::rotateX(glm::vec3{this->frontVector.x, 1.f, 0.f}, roll)
	)); // @warn @todo . roll 

	// Up
	this->upVector = glm::normalize(glm::cross(this->rightVector, this->frontVector));
}

void Camera::move(glm::vec3 offset)
{
	this->position += offset;
	this->updateViewMatrix();
}

void Camera::rotate(glm::vec3 offset)
{
	this->rotation += offset;
	this->updateDirectionalVectors();
	this->updateViewMatrix();
}

void Camera::faceTo(glm::vec3 position)
{
	this->setFrontVector(glm::normalize(this->position - position));
}

// @todo . dobrać lepszą nazwę niż ta od Piotrka (Makłowicza) Karskiego - relativeMove? 
void Camera::bartek(glm::vec3 direction)
{
	this->position += (
		direction.x * this->rightVector	+
		direction.y * this->upVector 	+
		direction.z * this->frontVector
	);
	this->updateViewMatrix();
}

}
