#pragma once

#include <string>

#include <glad/glad.h>

namespace sf3d
{

class Shader
{
public:

	enum ShaderType_e : GLenum
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	Shader() = default;

	Shader(const Shader&) = delete;
	Shader& operator = (const Shader&) = delete;

	Shader(Shader&&) = delete;
	Shader& operator = (Shader&&) = delete;

	~Shader();

	bool loadFromFile(const std::string& filePath, ShaderType_e type);

	operator GLuint () const;

private:

	void _destroy();

	bool _compile(const std::string& filePath, ShaderType_e type);

	GLuint _program;

};

}
