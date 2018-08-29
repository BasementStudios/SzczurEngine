#include "RenderTexture.hpp"

/** @file RenderTexture.cpp
 ** @author Tomasz (Knayder) Jatkowski 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <stdexcept>

#include <glm/vec2.hpp>

namespace sf3d {
	class ShaderProgram;
}
#include "RenderTarget.hpp"

namespace sf3d
{

/* Properties */
const Texture& RenderTexture::getTexture() const
{
	return this->texture;
}
Texture& RenderTexture::getTexture()
{
	return this->texture;
}



/* Operators */
RenderTexture::RenderTexture() 
{
	;
}
RenderTexture::RenderTexture(glm::uvec2 size, const ContextSettings& contextSettings, ShaderProgram* program)
{
	this->create(size, contextSettings, program);
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
void RenderTexture::create(glm::uvec2 size, const ContextSettings& contextSettings, ShaderProgram* program)
{
	RenderTarget::create(size, contextSettings, program);

	// If same (or no changes), do nothing
	if (this->texture.getSize() == size && (contextSettings == ContextSettings::None || this->contextSettings == contextSettings) ) {
		// @todo ? clear?
		return;
	}
	
	// Make sure to delete for recreation
	glDeleteFramebuffers(1, &(this->FBO));
	glDeleteRenderbuffers(1, &(this->RBO));

	// Generate buffers
	glGenFramebuffers(1, &(this->FBO));
	glGenRenderbuffers(1, &(this->RBO));

	// FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	{
		// Texture
		this->texture.create(this->size, this->contextSettings, TextureTarget::Simple2D);
		this->texture.bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureTarget::Simple2D, this->texture.getID(), 0);
		this->texture.unbind();

		// RenderBuffer
		glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, this->contextSettings.getRenderInternalFormat(), this->size.x, this->size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, this->contextSettings.getRenderAttachmentType(), GL_RENDERBUFFER, this->RBO);

		// Checks
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			throw std::runtime_error("Framebuffer creation error.");
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool RenderTexture::setActive(bool /*states*/)
{
	if (this->FBO) {
		glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
		return true;
	}
	return false;
}

}
