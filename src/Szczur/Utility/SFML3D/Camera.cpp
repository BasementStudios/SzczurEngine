#include "Camera.hpp"

/** @file Camera.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp> // radians, cos, sin
#include <glm/gtc/matrix_transform.hpp> // perspective, ortho

#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Utility/Logger.hpp"

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

// RenderDistance
float Camera::getRenderDistance() const
{
	return this->renderDistance;
}
void Camera::setRenderDistance(float maxRenderDistance, float minRenderDistance)
{
	this->renderDistance = maxRenderDistance;
	// @todo , minRenderDistance
	this->updateProjectionMatrix();
}

// ProjectionRatio
float Camera::getRatio() const
{
	return this->projectionRatio;
}
void Camera::setRatio(float ratio)
{
	this->projectionRatio = ratio;
	this->updateProjectionMatrix();
}

// ProjectionType
ProjectionType Camera::getProjectionType() const
{
	return this->projectionType;
}
const ProjectionData& Camera::getProjectionData() const
{
	return this->projectionData;
}

void Camera::setProjection(ProjectionType type, const ProjectionData& data)
{
	this->projectionType = type;
	switch (this->projectionType) {
		case ProjectionType::Perspective:
		{
			this->projectionData.perspective.setFOV(data.perspective.FOVy, this->projectionRatio);
		}
		break;

		default:
		{
			this->projectionData = data;
		}
	}
	this->updateProjectionMatrix();
}
void Camera::setProjection(const PerspectiveData& data) {
	this->projectionType = ProjectionType::Perspective;
	this->projectionData.perspective.setFOV(data.FOVy, this->projectionRatio);
	this->updateProjectionMatrix();
}
void Camera::setProjection(const OrthographicData& data) {
	this->projectionType = ProjectionType::Orthographic;
	this->projectionData.orthographic = data;
	this->updateProjectionMatrix();
}

// ProjectionMatrix
glm::mat4 Camera::getProjectionMatrix() const
{
	return this->projectionMatrix; 
}



/* Operators */
Camera::Camera(glm::vec3 position, glm::vec3 rotation, float projectionRatio, PerspectiveData data)
:	position(position), rotation(rotation), projectionRatio(projectionRatio)
{
	this->updateDirectionalVectors();
	this->updateViewMatrix();

	this->setProjection(data);
}
Camera::Camera(glm::vec3 position, glm::vec3 rotation, float projectionRatio, OrthographicData data)
:	position(position), rotation(rotation), projectionRatio(projectionRatio)
{
	this->updateDirectionalVectors();
	this->updateViewMatrix();

	this->setProjection(data);
}



/* Methods */
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

void Camera::updateProjectionMatrix()
{
	switch (this->projectionType) {
		case ProjectionType::Perspective: {
			this->projectionMatrix = glm::perspective(
				glm::radians(this->projectionData.perspective.FOVy), 
				this->projectionRatio, 
				0.1f, this->renderDistance
			);
		} break;

		case ProjectionType::Orthographic: {
			this->projectionMatrix = glm::ortho(
				this->projectionData.orthographic.left,
				this->projectionData.orthographic.right,
				this->projectionData.orthographic.bottom,
				this->projectionData.orthographic.top,
				0.1f, this->renderDistance
			);
		}; break;
	}
}

}
