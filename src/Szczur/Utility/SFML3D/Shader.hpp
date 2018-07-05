#pragma once

#include "Szczur/Config.hpp"

#ifdef EDITOR
#   include <string>
#endif // EDITOR

#include <glad/glad.h>

namespace sf3d
{

class Shader
{
public:

	enum ShaderType
	{
		Vertex = GL_VERTEX_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		TessControl = GL_TESS_CONTROL_SHADER,
		TessEvaluation = GL_TESS_EVALUATION_SHADER,
		Compute = GL_COMPUTE_SHADER
	};

	///
	Shader() = default;

	///
	Shader(const Shader&) = delete;

	///
	Shader& operator = (const Shader&) = delete;

	///
	Shader(Shader&& rhs) noexcept;

	///
	Shader& operator = (Shader&& rhs) noexcept;

	///
	~Shader();

	///
	bool loadFromFile(ShaderType type, const char* filePath);

	///
	bool loadFromMemory(ShaderType type, const void* data, GLint size = -1);

	///
	bool isValid() const;

	///
	GLuint getNativeHandle() const;

	#ifdef EDITOR

	ShaderType _type;
	std::string _filePath;
	const void* _dataPtr = nullptr;
	GLint _dataSize = 0;

	///
	bool _reload();

	#endif // EDITOR

private:

	///
	void _destroy();

	///
	bool _compile(ShaderType type, const char* data, GLint size);

	GLuint _shader = 0;

};

}
