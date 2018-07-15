#include "LightPoint.hpp"

/** @file LightPoint.cpp
 ** @description 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec3.hpp>

namespace sf3d
{

/* Properties */
/// Color
glm::vec3 LightPoint::getColor() const
{
	return this->color;
}
void LightPoint::setColor(const glm::vec3& color)
{
	this->color = color;
}

/// Attenuation
LightPoint::Attenuation LightPoint::getAttenuation() const
{
	return this->attenuation;
}
void LightPoint::setAttenuation(const Attenuation& attenuation)
{
	this->attenuation = attenuation;
}
void LightPoint::setAttenuation(const glm::vec3& attenuation)
{
	this->attenuation.constant	= attenuation.x;
	this->attenuation.linear	= attenuation.y;
	this->attenuation.quadratic	= attenuation.z;
}

/// Ambient factor
glm::vec3 LightPoint::getAmbientFactor() const
{
	return this->ambientFactor;
}
void LightPoint::setAmbientFactor(const glm::vec3& factor)
{
	this->ambientFactor = factor;
}

/// Diffuse factor
glm::vec3 LightPoint::getDiffuseFactor() const
{
	return this->diffuseFactor;
}
void LightPoint::setDiffuseFactor(const glm::vec3& factor)
{
	this->diffuseFactor = factor;
}

/// Specular factor
glm::vec3 LightPoint::getSpecularFactor() const
{
	return this->specularFactor;
}
void LightPoint::setSpecularFactor(const glm::vec3& factor)
{
	this->specularFactor = factor;
}

}
