#pragma once

/** @file RenderTexture.hpp
 ** @author Tomasz (Knayder) Jatkowski 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec2.hpp>

namespace sf3d {
	class ShaderProgram;
}
#include "RenderTarget.hpp"
#include "Texture.hpp"
#include "ContextSettings.hpp"

namespace sf3d 
{

/// Specialization of render target which can be used as texture
class RenderTexture : public RenderTarget
{
	/* Variables */
protected:
	GLuint FBO {0u}; // @todo ? maybe FrameBuffer and RenderBuffer should become separate classes?
	GLuint RBO {0u};
	Texture texture;
	
	/* Properties */
public:
	/// Provides access to texture 
	const Texture& getTexture() const;
	Texture& getTexture();



	/* Operators */
public:
	RenderTexture();
	RenderTexture(glm::uvec2 size, const ContextSettings& contextSettings = ContextSettings::Default, ShaderProgram* program = nullptr);
	~RenderTexture();



	/* Methods */
public:
	void create(glm::uvec2 size, const ContextSettings& contextSettings = ContextSettings::None, ShaderProgram* program = nullptr);
	
	/// Selects this render context as current for graphics
	virtual bool setActive(bool states = true) override; // @todo ? setActive false true ma sens - wejscie i wyjscie!
};

}
