#pragma once

/** @file RenderTexture.hpp
 ** @author Tomasz (Knayder) Jatkowski 
 **/

#include <glm/vec2.hpp>

namespace sf3d {
	class ShaderProgram;
}
#include "RenderTarget.hpp"
#include "Texture.hpp"

namespace sf3d 
{

class RenderTexture : public RenderTarget
{
	/* Variables */
private:
	GLuint _FBO{0u};
	Texture _texture;
	GLuint _RBO{0u};


	
	/* Properties */
public:
	/// 
	Texture* getTexture();



	/* Operators */
public:
	RenderTexture();
	RenderTexture(glm::uvec2 size, ShaderProgram* program);
	~RenderTexture();



	/* Methods */
public:
	void create(glm::uvec2 size, ShaderProgram* program);
	
	virtual bool _setActive(bool states = true) override;
};

}
