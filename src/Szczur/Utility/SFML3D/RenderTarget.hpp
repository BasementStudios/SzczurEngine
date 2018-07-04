#pragma once

/** @file RenderTarget.hpp
 ** @author Tomasz (Knayder) Jatkowski 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <glad/glad.h> // GLbitfield

namespace sf {
	class Color;
}
#include "RenderStates.hpp"
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
private:
	glm::uvec2 size;

	RenderStates defaultStates;
	
	Camera* camera {nullptr};
	Camera* defaultCamera;
	
	glm::mat4 projectionMatrix;

	float renderDistance {100.f};

	float positionFactor;

	float FOVy;
	float FOVx;
	float halfFOVxTan;
	float halfFOVyTan;

	std::vector<LightPoint*> lightPoints;

	char uniformNameBuffer[64];



	/* Properties */
public:
	/// Default render states, used if no other provided to `draw` functions
	RenderStates getDefaultRenderStates() const;
	void setDefaultRenderStates(const RenderStates& states);
	void setDefaultShaderProgram(ShaderProgram* program);

	/// Current camera object which define what to render
	Camera* getCamera();
	const Camera* getCamera() const;
	void setCamera(Camera* camera);
	void setCamera(Camera& camera);

	/// Field of view
	float getFOV() const;
	void setFOV(float fov);

	/// Render distance
	void setRenderDistance(float maxRenderDistance, float minRenderDistance = 0.1f);
	float getRenderDistance() const; // @todo ? co z `minRenderDistance`?
	


	/* Operators */
public:
	RenderTarget();

	RenderTarget(const glm::uvec2& size, float FOV = 45.f, ShaderProgram* program = nullptr);

	~RenderTarget();



	/* Methods */
public:
	void create(const glm::uvec2& size, float FOV = 45.f, ShaderProgram* program = nullptr);
	
	virtual bool _setActive(bool state = true);

	void updatePerspective();

	/// Helper function to scale matrix coords propertly
	glm::mat4 scaleMatrixCoords(glm::mat4 matrix);

	// Clearing
	void clear(float r, float g, float b, float a, GLbitfield flags);
	void clear(const sf::Color& color, GLbitfield flags);

	// Drawing drawables
	void draw(const Drawable& drawable, const RenderStates& states);
	void draw(const Drawable& drawable);

	// Drawing vertices
	void draw(const VertexArray& vertices, const RenderStates& states);
	void draw(const VertexArray& vertices);
	
	// Interaction
	Linear getLinerByScreenPos(const glm::vec2& pos) const;
	
	// Light points
	void resetLightPoints();
	void registerLightPoint(LightPoint* lightPoint);
	void applyLightPoints(ShaderProgram* shaderProgram);
};

}
