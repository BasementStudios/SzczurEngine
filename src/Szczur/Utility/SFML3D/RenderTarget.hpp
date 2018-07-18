#pragma once

/** @file RenderTarget.hpp
 ** @author Tomasz (Knayder) Jatkowski 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h> // GLbitfield
#include <SFML/Graphics/Color.hpp>

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
protected:
	glm::uvec2 size;

	RenderStates defaultStates;
	
	Camera* camera {nullptr};
	Camera* defaultCamera {nullptr};

public:
	float positionFactor;

protected:
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
	


	/* Operators */
	// Protected to avoid construction (it is only base type).
protected:
	RenderTarget();
	~RenderTarget();

	RenderTarget(glm::uvec2 size, ShaderProgram* program = nullptr);



	/* Methods */
protected:
	void create(glm::uvec2 size, ShaderProgram* program = nullptr);

	virtual bool _setActive(bool state = true) = 0;

public:
	/// Helper function to scale matrix coords propertly
	glm::mat4 scaleMatrixCoords(glm::mat4 matrix);

	// Clearing
	void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 0.f, GLbitfield flags = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	void clear(const sf::Color& color = sf::Color::Transparent, GLbitfield flags = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Drawing drawables
	void draw(const Drawable& drawable, const RenderStates& states);
	void draw(const Drawable& drawable);

	// Drawing vertices
	void draw(const VertexArray& vertices, const RenderStates& states);
	void draw(const VertexArray& vertices);

    // "Simple draw" 
    void simpleDraw(const VertexArray& vertices, RenderStates states); 
    void simpleDraw(const VertexArray& vertices); 
	
	// Interaction
	Linear getLinearByScreenPosition(glm::vec2 pos) const;
	
	// Light points
	void resetLightPoints();
	void registerLightPoint(LightPoint* lightPoint);
	void applyLightPoints(ShaderProgram* shaderProgram);
};

}
