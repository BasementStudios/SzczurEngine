#pragma once

#include "RenderTarget.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include "ShaderProgram.hpp"

namespace sf3d {
	class RenderTexture : public RenderTarget {
	public:
		RenderTexture(ShaderProgram* program, const glm::uvec2& size, float FOV = 45.f);
		~RenderTexture();
		Texture* getTexture();
	private:
		GLuint _FBO{0u};
		Texture _texture;
		GLuint _RBO{0u};

		virtual bool _setActive(bool states = true) override;
	};
}