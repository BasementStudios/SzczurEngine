#pragma once

#include "glad.h"

#include <glm/glm.hpp>

#include "Shader.hpp"

namespace sf3d
{

class ShaderProgram
{
public:

	ShaderProgram() = default;

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator = (const ShaderProgram&) = delete;

	ShaderProgram(ShaderProgram&&) = delete;
	ShaderProgram& operator = (ShaderProgram&&) = delete;

	~ShaderProgram();

	template<typename... Ts>
	bool linkShaders(const Ts&... shaders) {
		_program = glCreateProgram();

		(glAttachShader(_program, shaders), ...);

		glLinkProgram(_program);

		(glDetachShader(_program, shaders), ...);

		// Handle linking result
		GLint success;
		glGetProgramiv(_program, GL_LINK_STATUS, &success);
		if (success != GL_TRUE) {
			GLchar infoLog[512];
			glGetProgramInfoLog(_program, sizeof(infoLog) * sizeof(GLchar), nullptr, infoLog);
			LOG_ERROR("Unable to link shader program\n", infoLog);
			_destroy();
			return false;
		}

		return true;
	}


	void use() const;

	void setUniform(const char* name, bool x);
	void setUniform(const char* name, const glm::bvec2& xy);
	void setUniform(const char* name, const glm::bvec3& xyz);
	void setUniform(const char* name, const glm::bvec4& xyzw);

	void setUniform(const char* name, float x);
	void setUniform(const char* name, const glm::vec2& xy);
	void setUniform(const char* name, const glm::vec3& xyz);
	void setUniform(const char* name, const glm::vec4& xyzw);

	void setUniform(const char* name, int x);
	void setUniform(const char* name, const glm::ivec2& xy);
	void setUniform(const char* name, const glm::ivec3& xyz);
	void setUniform(const char* name, const glm::ivec4& xyzw);

	void setUniform(const char* name, unsigned int x);
	void setUniform(const char* name, const glm::uvec2& xy);
	void setUniform(const char* name, const glm::uvec3& xyz);
	void setUniform(const char* name, const glm::uvec4& xyzw);

	void setUniform(const char* name, const glm::mat2x2& mat, bool transpose = GL_FALSE);
	void setUniform(const char* name, const glm::mat3x3& mat, bool transpose = GL_FALSE);
	void setUniform(const char* name, const glm::mat4x4& mat, bool transpose = GL_FALSE);

	operator GLuint () const;

private:

	void _destroy();

	GLuint _program;

};

}
