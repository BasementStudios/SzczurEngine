#include "RenderTexture.hpp"

#include <iostream>

namespace sf3d {
	RenderTexture::RenderTexture(const char* vertexPath, const char* fragmentPath, const glm::uvec2 & size, float FOV) :
	RenderTarget(vertexPath, fragmentPath, size, FOV) {
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
			glBindRenderbuffer(GL_RENDERBUFFER, NULL);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "FRAMEBUFFER CREATION ERROR\n";

		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
		//Framebuffer
	}
	RenderTexture::~RenderTexture() {
		if(_FBO)
			glDeleteFramebuffers(1, &_FBO);
		if(_RBO)
			glDeleteRenderbuffers(1, &_RBO);
	}

	Texture* RenderTexture::getTexture() {
		return &_texture;
	}
	bool RenderTexture::_setActive(bool states) {
		if(_FBO) {
			glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
			return true;
		}
		return false;
	}
}