#pragma once

/** @file RenderTarget.hpp
 ** @author Tomasz (Knayder) Jatkowski 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h> // GLbitfield
#include <SFML/Graphics/Color.hpp>

#include "RenderStates.hpp"
#include "Texture.hpp"
#include "ContextSettings.hpp"
#include "Camera.hpp"
namespace sf3d {
	class VertexArray;
	class Drawable;
	class LightPoint;
	class Linear;
	class ShaderProgram;
}

namespace sf3d
{

/// Performs render operations
class RenderTarget
{
	/* Variables */
protected:
	glm::uvec2 size; // @todo ? Shouldn't be obtained by higher implementation (window/texture size)??

	RenderStates defaultStates;
	
	ContextSettings contextSettings = ContextSettings::Default;

	Camera* camera {nullptr};
	Camera* defaultCamera {nullptr};

	// Multisampling/antialiasing support
	GLuint mutlisampledFBO {0u};
	GLuint mutlisampledRBO {0u};
	Texture multisampledTexture;

public:
	float positionFactor;

protected:
	// Lighting
	std::vector<LightPoint*> lightPoints;

private:
	char uniformNameBuffer[64];



	/* Properties */
public:
	/// Default render states, used if no other provided to `draw` functions
	RenderStates getDefaultRenderStates() const;
	void setDefaultRenderStates(const RenderStates& states);
	void setDefaultShaderProgram(ShaderProgram* program);
	void setDefaultShaderProgram(ShaderProgram& program);

	/// Current camera object which define what to render
	Camera* getCamera();
	const Camera* getCamera() const;
	void setCamera(Camera* camera);
	void setCamera(Camera& camera);

	/// Multisampling level, one (1) means disabled.
	unsigned int getMultisamplingLevel() const;
	void setMultisamplingLevel(unsigned int samples = 4u);
	


	/* Operators */
	// Protected to avoid construction (it is only base type).
protected:
	RenderTarget();
	~RenderTarget();

	RenderTarget(glm::uvec2 size, const ContextSettings& contextSettings = ContextSettings::Default, ShaderProgram* program = nullptr);



	/* Methods */
protected:
	void create(glm::uvec2 size, const ContextSettings& contextSettings = ContextSettings::None, ShaderProgram* program = nullptr);

	virtual bool setActive(bool state = true) = 0;

protected:
	// Helper functions for managing multisampling 
	bool multisamplingSetActive(bool state = true);
	bool multisamplingBlitFramebuffer();

public:
	/// Helper function to scale matrix coords propertly
	glm::mat4 scaleMatrixCoords(glm::mat4 matrix);

	// Clearing
	void clear(const glm::vec4& color, GLbitfield flags = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	[[deprecated("SF3D should operate on its own color class in the future, but at now it is `glm::vec4`.")]]
	void clearSFML(const sf::Color& color = sf::Color::Transparent, GLbitfield flags = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Drawing drawables
	void draw(const Drawable& drawable, const RenderStates& states);
	void draw(const Drawable& drawable);

	// Drawing vertices
	void draw(const VertexArray& vertices, const RenderStates& states);
	void draw(const VertexArray& vertices);
	
	// Interaction
	Linear getLinearByScreenPosition(glm::vec2 pos) const;
	
	// Light points
	void resetLightPoints();
	void registerLightPoint(LightPoint* lightPoint);
	void applyLightPoints(ShaderProgram* shaderProgram);
};

}
