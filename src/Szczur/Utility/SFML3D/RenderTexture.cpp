#include "RenderTexture.hpp"

/** @file RenderTexture.cpp
 ** @author Tomasz (Knayder) Jatkowski 
 **/

#include <iostream> // cout

#include <glm/vec2.hpp>

namespace sf3d {
	class ShaderProgram;
}
#include "RenderTarget.hpp"

namespace sf3d
{

RenderTexture::RenderTexture() 
{
	;
}
RenderTexture::RenderTexture(glm::uvec2 size, ShaderProgram* program)
{
	create(size, program);
}
RenderTexture::~RenderTexture() {
	if(_FBO)
		glDeleteFramebuffers(1, &_FBO);
	if(_RBO)
		glDeleteRenderbuffers(1, &_RBO);
}

void RenderTexture::create(glm::uvec2 size, ShaderProgram* program) {
	RenderTarget::create(size, program);

	glGenFramebuffers(1, &_FBO);
	glGenRenderbuffers(1, &_RBO);

	//Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	//Texture
	_texture.create(size);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture.getID(), 0);

	//RenderBuffer
	glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FRAMEBUFFER CREATION ERROR\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Framebuffer
}

Texture* RenderTexture::getTexture() {
	return &_texture;
}
bool RenderTexture::_setActive(bool /*states*/) {
	if(_FBO) {
		glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
		return true;
	}
	return false;
}

}
