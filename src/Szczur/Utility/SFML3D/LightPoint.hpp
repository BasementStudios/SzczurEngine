#pragma once

/** @file LightPoint.hpp
 ** @description 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec3.hpp>

#include "Transformable.hpp"

namespace sf3d
{

class LightPoint : public Transformable
/// Stores data related to point light.
{
    /* Types */
public:
    struct Attenuation {
        float constant;
        float linear;
        float quadratic;
    };



    /* Variables */
public:
    /// Color of the light
	glm::vec3 color;

	/// Attenuation
    Attenuation attenuation;
    
	// Light factors
    glm::vec3 ambientFactor;
    glm::vec3 diffuseFactor;
    glm::vec3 specularFactor;



    /* Properties */
public:
    /// Color
    glm::vec3 getColor() const;
    void setColor(const glm::vec3& color);
    
    /// Attenuation
    Attenuation getAttenuation() const;
    void setAttenuation(const Attenuation& attenuation);
    void setAttenuation(const glm::vec3& attenuation);
    
    /// Ambient factor
    glm::vec3 getAmbientFactor() const;
    void setAmbientFactor(const glm::vec3& factor);

    /// Diffuse factor
    glm::vec3 getDiffuseFactor() const;
    void setDiffuseFactor(const glm::vec3& factor);
    
    /// Specular factor
    glm::vec3 getSpecularFactor() const;
    void setSpecularFactor(const glm::vec3& factor);
};

}
