#pragma once

/** @file Camera.hpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Projections.hpp"

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
	
	// Projection data
	float renderDistance;
	float projectionRatio;
	ProjectionType projectionType;
	ProjectionData projectionData;

	// Buffering matrixes products, to avoid continuous recalculations.
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	

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

	/// View matrix used to render camera vision
	glm::mat4 getViewMatrix() const;

	/// Render distance
	void setRenderDistance(float maxRenderDistance, float minRenderDistance = 0.1f);
	float getRenderDistance() const; // @todo ? co z `minRenderDistance`?

	/// Ratio 
	float getRatio() const;
	void setRatio(float fov);

	/// Data and type of projection used by camera while rendering its view
	ProjectionType getProjectionType() const;
	const ProjectionData& getProjectionData() const;
	void setProjection(ProjectionType type, const ProjectionData& data = PerspectiveData{45.f, 1125.f, 0.f, 720.f});
	void setProjection(const PerspectiveData& data);
	void setProjection(const OrthographicData& data);

	/// Projection matrix used to render camera vision
	glm::mat4 getProjectionMatrix() const;



	/* Operators */
public:
	Camera(
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
		float projectionRatio = (16.f / 9.f),
		PerspectiveData data = {45.f, 0.f, 0.f, 0.f}
	);
	Camera(
		glm::vec3 position,
		glm::vec3 rotation,
		float projectionRatio,
		OrthographicData data
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
	void updateDirectionalVectors();

	void updateViewMatrix();

	void updateProjectionMatrix();
};

}
