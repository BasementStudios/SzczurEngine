#include "RenderTarget.hpp"
#include "Szczur/Utility/Logger.hpp"

/** @file RenderTarget.cpp
 ** @author Tomasz (Knayder) Jatkowski
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <cstdio> // snprintf
#include <stdexcept>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glad/glad.h>

#include <SFML/Graphics/Color.hpp>

#include "RenderStates.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "VertexArray.hpp"
#include "Drawable.hpp"
#include "LightPoint.hpp"
#include "Geometry/Linear.hpp"
#include "ShaderProgram.hpp"

namespace sf3d
{

/* Properties */
// DefaultRenderStates
RenderStates RenderTarget::getDefaultRenderStates() const
{
	return this->defaultStates;
}
void RenderTarget::setDefaultRenderStates(const RenderStates& states)
{
	this->defaultStates = states;
}

void RenderTarget::setDefaultShaderProgram(ShaderProgram* program)
{
	this->defaultStates.shader = program;
}
void RenderTarget::setDefaultShaderProgram(ShaderProgram& program)
{
	this->defaultStates.shader = &program;
}

// Camera
Camera* RenderTarget::getCamera()
{
	return this->camera;
}
const Camera* RenderTarget::getCamera() const
{
	return this->camera;
}
void RenderTarget::setCamera(Camera* camera)
{
	if (camera) {
		this->camera = camera;
	}
	else {
		this->camera = this->defaultCamera;
	}
}
void RenderTarget::setCamera(Camera& camera)
{
	this->setCamera(&camera);
}



/* Operators */
RenderTarget::RenderTarget()
{}

RenderTarget::RenderTarget(glm::uvec2 size, ShaderProgram* program)
{
	this->create(size, program);
}

RenderTarget::~RenderTarget()
{
	if (this->defaultCamera) {
		delete defaultCamera;
	}
}



/* Methods */
void RenderTarget::create(glm::uvec2 size, ShaderProgram* program)
{
	this->size = size;

	if (program) {
		this->setDefaultShaderProgram(program);
	}

	if (!this->defaultCamera) {
		this->camera = this->defaultCamera = new Camera(
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, 0.f),
			(static_cast<float>(this->size.x) / static_cast<float>(this->size.y))
		);
	}

	// @warn @todo . It shouldn't be here...
	this->positionFactor = 2.f / static_cast<float>(this->size.y);
}

bool RenderTarget::_setActive([[maybe_unused]] bool state)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	throw std::runtime_error("RenderTarget::_setActive not overloaded!"); 
	return false;
}

/// Helper function to scale matrix coords propertly
glm::mat4 RenderTarget::scaleMatrixCoords(glm::mat4 matrix)
{
	matrix[3][0] *= this->positionFactor;
	matrix[3][1] *= this->positionFactor;
	matrix[3][2] *= this->positionFactor;
	return matrix;
}

// Clearing
void RenderTarget::clear(const glm::vec4& color, GLbitfield flags)
{
	if (this->_setActive()) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(flags);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
void RenderTarget::clearSFML(const sf::Color& color, GLbitfield flags)
{
	if (this->_setActive()) {
		glClearColor(
			static_cast<float>(color.r) / 255.f, 
			static_cast<float>(color.g) / 255.f, 
			static_cast<float>(color.b) / 255.f, 
			static_cast<float>(color.a) / 255.f
		);
		glClear(flags);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

// Drawing drawables in perspective projection
void RenderTarget::draw(const Drawable& drawable, const RenderStates& states)
{
	drawable.draw(*this, states);
}
void RenderTarget::draw(const Drawable& drawable)
{
	this->draw(drawable, this->defaultStates);
}

// Drawing vertices in perspective projection
void RenderTarget::draw(const VertexArray& vertices, const RenderStates& states)
{
	if (vertices.getSize() > 0 && this->_setActive()) {
		vertices.update();

		// Shader selection
		ShaderProgram* shaderProgram = (states.shader ? states.shader : this->defaultStates.shader);
		if (!(shaderProgram && shaderProgram->isValid())) {
			throw std::runtime_error("No shader available for rendering!");
		}

		// Shader configuration
		{
			glUseProgram(shaderProgram->getNativeHandle());

			// For futher testing...
			// LOG_INFO("vertices[0] = ", vertices[0].position, " ", vertices[0].color, " ", vertices[0].texCoord);
			// LOG_INFO("vertices[1] = ", vertices[1].position, " ", vertices[1].color, " ", vertices[1].texCoord);
			// LOG_INFO("vertices[2] = ", vertices[2].position, " ", vertices[2].color, " ", vertices[2].texCoord);
			// LOG_INFO("vertices[3] = ", vertices[3].position, " ", vertices[3].color, " ", vertices[3].texCoord);
			// LOG_INFO("model       = ", scaleMatrixCoords(states.transform.getMatrix()));
			// LOG_INFO("view        = ", scaleMatrixCoords(camera->getViewMatrix()));
			// LOG_INFO("projection  = ", camera->getProjectionMatrix());
			// LOG_INFO("texture ID  = ", (states.texture ? states.texture->getID() : -1));

			// Model, view. projection matrixes
			shaderProgram->setUniform("model",			scaleMatrixCoords(states.transform.getMatrix()));
			shaderProgram->setUniform("view",			scaleMatrixCoords(camera->getViewMatrix()));
			shaderProgram->setUniform("projection", 	camera->getProjectionMatrix());
			shaderProgram->setUniform("positionFactor", this->positionFactor);

			if (states.texture) { // @todo ? Może dodać `Lightable`, a nie oświetlać tylko oteksturowane...
				shaderProgram->setUniform("hasTexture", true);
				shaderProgram->setUniform("isObject", true);

				// Material
				{
					// Diffuse
					glActiveTexture(GL_TEXTURE0);
					states.texture->bind();
					shaderProgram->setUniform("material.diffuseTexture", 0);
					shaderProgram->setUniform("texture", 0);

					// Specular // @todo . specular
					//aderProgram->setUniform("material.specularTexture", ???.texture->getID());
					//aderProgram->setUniform("material.shininess", ???.shininess);
				}

				// Lighting
				shaderProgram->setUniform("cameraPosition", camera->getPosition());
				shaderProgram->setUniform("basicAmbient", glm::vec3{0.1f, 0.1f, 0.1f});
				applyLightPoints(shaderProgram);
			}
		}

		// Pass the vertices
		vertices.bind();
		glDrawArrays(vertices.getPrimitiveType(), 0, vertices.getSize());
		vertices.unbind();

		// Unbind testures if any
		if (states.texture) {
			states.texture->unbind();
		}
	}
}
void RenderTarget::draw(const VertexArray& vertices)
{
	this->draw(vertices, this->defaultStates);
}

// Interaction
Linear RenderTarget::getLinearByScreenPosition(glm::vec2 screenPosition) const
{
	const ProjectionData& projectionData = this->getCamera()->getProjectionData();
	glm::vec3 rotation;

	switch (this->getCamera()->getProjectionType()) {
		case ProjectionType::Perspective:
		{
			const PerspectiveData& perspectiveData = projectionData.perspective;

			float x = glm::atan(
				( 2.f * (screenPosition.x / static_cast<float>(this->size.x)) - 1.f) * perspectiveData.halfFOVxTan
			);
			float y = glm::atan(
				(-2.f * (screenPosition.y / static_cast<float>(this->size.y)) + 1.f) * perspectiveData.halfFOVyTan
			);

			float siny = glm::sin(y);
			float cosy = glm::cos(y);
			float sinx = glm::sin(x);
			float cosx = glm::cos(x);

			rotation = {cosy * sinx, siny * cosx, -cosy * cosx};
			rotation = glm::rotateX(rotation, glm::radians(this->camera->getRotation().x));
			rotation = glm::rotateY(rotation, glm::radians(this->camera->getRotation().y));
		}
		break;

		case ProjectionType::Orthographic:
		{
			throw std::logic_error("getLinearByScreenPosition: Orhographics projection type not implemented yet.");
			// @todo .
		}
		break;
	}

	return Linear(this->camera->getPosition(), rotation);
}

// Light points
void RenderTarget::resetLightPoints()
{
	this->lightPoints.clear();
}
void RenderTarget::registerLightPoint(LightPoint* lightPoint)
{
	this->lightPoints.push_back(lightPoint);
}
void RenderTarget::applyLightPoints(ShaderProgram* shaderProgram)
{
	unsigned int i = 0;
	for (const LightPoint* lightPoint : this->lightPoints) {
		std::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].position", i);
		shaderProgram->setUniform(this->uniformNameBuffer, lightPoint->getPosition() * this->positionFactor);
		std::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].color", i);
		shaderProgram->setUniform(this->uniformNameBuffer, lightPoint->getColor());
		std::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].attenuation.constant", i);
		shaderProgram->setUniform(this->uniformNameBuffer, lightPoint->attenuation.constant);
		std::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].attenuation.linear", i);
		shaderProgram->setUniform(this->uniformNameBuffer, lightPoint->attenuation.linear);
		std::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].attenuation.quadratic", i);
		shaderProgram->setUniform(this->uniformNameBuffer, lightPoint->attenuation.quadratic);
		std::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].ambientFactor", i);
		shaderProgram->setUniform(this->uniformNameBuffer, lightPoint->getAmbientFactor());
		std::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].diffuseFactor", i);
		shaderProgram->setUniform(this->uniformNameBuffer, lightPoint->getDiffuseFactor());
		//d::snprintf(this->uniformNameBuffer, 64, "pointLights[%u].specularFactor", i); // @todo . specular
		//aderProgram->setUniform(this->uniformNameBuffer, lightPoint->getSpecularFactor());
		i++;
	}
	shaderProgram->setUniform("pointLightsLength", i);
}

}
