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

/* Properties */
Texture* RenderTexture::getTexture()
{
	return &(this->texture);
}



/* Operators */
RenderTexture::RenderTexture() 
{
	;
}
RenderTexture::RenderTexture(glm::uvec2 size, ShaderProgram* program)
{
	this->create(size, program);
}
RenderTexture::~RenderTexture()
{
	if (this->FBO) {
		glDeleteFramebuffers(1, &(this->FBO));
	}
	if (this->RBO) {
		glDeleteRenderbuffers(1, &(this->RBO));
	}
}



/* Methods */
void RenderTexture::create(glm::uvec2 size, ShaderProgram* program)
{
	RenderTarget::create(size, program);

	// If same size, do nothing
	if (this->texture.getSize() == size) {
		return;
	}

	// Delete first, if it is recreation
	if (this->FBO) {
		glDeleteFramebuffers(1, &(this->FBO));
	}
	if (this->RBO) {
		glDeleteRenderbuffers(1, &(this->RBO));
	}

	// Generate buffers
	glGenFramebuffers(1, &(this->FBO));
	glGenRenderbuffers(1, &(this->RBO));

	// FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, (this->FBO));
	{
		// Texture
		this->texture.create(size);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.getID(), 0);

		// RenderBuffer
		glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FRAMEBUFFER CREATION ERROR\n";
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool RenderTexture::_setActive(bool /*states*/)
{
	if (this->FBO) {
		glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
		return true;
	}
	return false;
}

}
