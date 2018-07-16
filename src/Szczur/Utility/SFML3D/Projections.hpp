#pragma once 

/** @file Projections.hpp
 ** @description This file contains structures and functions related to projections.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/trigonometric.hpp> // degress, radians, tan, atan

namespace sf3d
{

enum class ProjectionType {
	Perspective,
	Orthographic
	// @todo ? more
};

struct PerspectiveData {
	float FOVy;
	
	// Buffered to avoid some recalculations
	float FOVx;
	float halfFOVxTan;
	float halfFOVyTan;

	float getFOV() const 
	{
		return this->FOVy;
	}
	void setFOV(float fov, float ratio)
	{
		this->FOVy = fov;
		this->FOVx = glm::degrees(
			2 * glm::atan(glm::tan(glm::radians(this->FOVy / 2.f)) * ratio)
		);
		this->halfFOVxTan = glm::tan(glm::radians(this->FOVx / 2.f));
		this->halfFOVyTan = glm::tan(glm::radians(this->FOVy / 2.f));
	}
};

struct OrthographicData {
	float left;
	float right;
	float bottom;
	float top;
};

union ProjectionData {
	PerspectiveData  perspective;
	OrthographicData orthographic;

	ProjectionData() = default;
	ProjectionData(const PerspectiveData& data)  { this->perspective  = data; }
	ProjectionData(const OrthographicData& data) { this->orthographic = data; }
};

}
