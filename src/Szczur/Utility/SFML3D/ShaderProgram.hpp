#pragma once

#include <glad\glad.h>
#include "Shader.hpp"
namespace sf3d {
	class ShaderProgram {
	public:
		ShaderProgram() = delete;
		ShaderProgram(GLuint vShader, GLuint fShader);
		~ShaderProgram();

		void use() const;

		operator GLuint();
	private:
		GLuint _program;
	};
}
