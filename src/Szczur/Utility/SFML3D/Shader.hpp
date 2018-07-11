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

	using NativeHandle_t = GLuint;

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

	// Non-copyable
	Shader(const Shader&) = delete;
	Shader& operator = (const Shader&) = delete;

	// Movable
	Shader(Shader&& rhs) noexcept;
	Shader& operator = (Shader&& rhs) noexcept;

	///
	~Shader();

	///
	/// Loads and compiles from type and path
	void loadFromFile(ShaderType type, const char* filePath);
	void loadFromFile(ShaderType type, const std::string& filePath);

	/// Loads and compiles from memory 
	void loadFromMemory(ShaderType type, const char* data, GLint size = -1);
	void loadFromMemory(ShaderType type, const std::string& data);

	/// Checks is shader vaild
	bool isValid() const;

	/// Return native handler of shader
	NativeHandle_t getNativeHandle() const;

#ifdef EDITOR
	ShaderType _type;
	std::string _filePath;
	const char* _dataPtr = nullptr;
	GLint _dataSize = 0;

	///
	void _reload();
#endif // EDITOR

private:

	///
	void _destroy();

	///
	void _compile(ShaderType type, const char* data, GLint size);

	NativeHandle_t _shader = 0;

};

}
