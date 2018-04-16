#include "ShaderProgram.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "Szczur/Utility/Logger.hpp"

namespace sf3d
{

ShaderProgram::~ShaderProgram()
{
	_destroy();
}

bool ShaderProgram::linkShaders(const Shader& first, const Shader& second)
{
	// Linking process
	_program = glCreateProgram();
	glAttachShader(_program, first);
	glAttachShader(_program, second);
	glLinkProgram(_program);
	glDetachShader(_program, first);
	glDetachShader(_program, second);

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

void ShaderProgram::use() const
{
	glUseProgram(_program);
}

void ShaderProgram::setUniform(const char* name, bool x)
{
	glUniform1i(glGetUniformLocation(_program, name), x);
}

void ShaderProgram::setUniform(const char* name, const glm::bvec2& xy)
{
	glUniform2i(glGetUniformLocation(_program, name), xy.x, xy.y);
}

void ShaderProgram::setUniform(const char* name, const glm::bvec3& xyz)
{
	glUniform3i(glGetUniformLocation(_program, name), xyz.x, xyz.y, xyz.z);
}

void ShaderProgram::setUniform(const char* name, const glm::bvec4& xyzw)
{
	glUniform4i(glGetUniformLocation(_program, name), xyzw.x, xyzw.y, xyzw.z, xyzw.w);
}

void ShaderProgram::setUniform(const char* name, float x)
{
	glUniform1f(glGetUniformLocation(_program, name), x);
}

void ShaderProgram::setUniform(const char* name, const glm::vec2& xy)
{
	glUniform2f(glGetUniformLocation(_program, name), xy.x, xy.y);
}

void ShaderProgram::setUniform(const char* name, const glm::vec3& xyz)
{
	glUniform3f(glGetUniformLocation(_program, name), xyz.x, xyz.y, xyz.z);
}

void ShaderProgram::setUniform(const char* name, const glm::vec4& xyzw)
{
	glUniform4f(glGetUniformLocation(_program, name), xyzw.x, xyzw.y, xyzw.z, xyzw.w);
}

void ShaderProgram::setUniform(const char* name, int x)
{
	glUniform1i(glGetUniformLocation(_program, name), x);
}

void ShaderProgram::setUniform(const char* name, const glm::ivec2& xy)
{
	glUniform2i(glGetUniformLocation(_program, name), xy.x, xy.y);
}

void ShaderProgram::setUniform(const char* name, const glm::ivec3& xyz)
{
	glUniform3i(glGetUniformLocation(_program, name), xyz.x, xyz.y, xyz.z);
}

void ShaderProgram::setUniform(const char* name, const glm::ivec4& xyzw)
{
	glUniform4i(glGetUniformLocation(_program, name), xyzw.x, xyzw.y, xyzw.z, xyzw.w);
}

void ShaderProgram::setUniform(const char* name, unsigned int x)
{
	glUniform1ui(glGetUniformLocation(_program, name), x);
}

void ShaderProgram::setUniform(const char* name, const glm::uvec2& xy)
{
	glUniform2ui(glGetUniformLocation(_program, name), xy.x, xy.y);
}

void ShaderProgram::setUniform(const char* name, const glm::uvec3& xyz)
{
	glUniform3ui(glGetUniformLocation(_program, name), xyz.x, xyz.y, xyz.z);
}

void ShaderProgram::setUniform(const char* name, const glm::uvec4& xyzw)
{
	glUniform4ui(glGetUniformLocation(_program, name), xyzw.x, xyzw.y, xyzw.z, xyzw.w);
}

void ShaderProgram::setUniform(const char* name, const glm::mat2x2& mat, bool transpose)
{
	glUniformMatrix2fv(glGetUniformLocation(_program, name), 1, transpose, glm::value_ptr(mat));
}

void ShaderProgram::setUniform(const char* name, const glm::mat3x3& mat, bool transpose)
{
	glUniformMatrix3fv(glGetUniformLocation(_program, name), 1, transpose, glm::value_ptr(mat));
}

void ShaderProgram::setUniform(const char* name, const glm::mat4x4& mat, bool transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(_program, name), 1, transpose, glm::value_ptr(mat));
}


ShaderProgram::operator GLuint () const
{
	return _program;
}

void ShaderProgram::_destroy()
{
	if (_program) {
		glDeleteProgram(_program);
	}
}

}
