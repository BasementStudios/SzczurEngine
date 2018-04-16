#include "Shader.hpp"

#include <fstream>
#include <iomanip>
#include <vector>

#include "Szczur/Utility/Logger.hpp"

namespace sf3d
{

bool getFileContent(const std::string& filePath, std::vector<char>& buffer)
{
	std::ifstream file{ filePath, std::ios_base::binary };

	if (file) {
		file.seekg(0, std::ios_base::end);
		if (auto size = file.tellg(); size > 0) {
			file.seekg(0, std::ios_base::beg);
			buffer.resize(static_cast<size_t>(size) + 1, '\0');
			file.read(&buffer[0], size);
			return true;
		}
	}

	return false;
}

Shader::~Shader()
{
	_destroy();
}

bool Shader::loadFromFile(const std::string& filePath, ShaderType_e type)
{
	return _compile(filePath, type);
}

Shader::operator GLuint () const
{
	return _program;
}

void Shader::_destroy()
{
	if (_program) {
		glDeleteShader(_program);		
	}
}

bool Shader::_compile(const std::string& filePath, ShaderType_e type)
{
	// Obtain shader code from file
	std::vector<char> buffer;

	if (!getFileContent(filePath, buffer)) {
		LOG_ERROR("Cannot get shader content from ", std::quoted(filePath));
		return false;
	}

	// Destroy previous program
	_destroy();

	// Create new program
	const char* src = buffer.data();
	_program = glCreateShader(type);
	glShaderSource(_program, 1, &src, nullptr);
	glCompileShader(_program);

	// Handle compilation result
	GLint success;
	glGetShaderiv(_program, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		GLchar infoLog[512];
		glGetShaderInfoLog(_program, sizeof(infoLog) * sizeof(GLchar), nullptr, infoLog);
		LOG_ERROR("Unable to compile shader ", std::quoted(filePath), '\n', infoLog);
		_destroy();
		return false;
	}

	LOG_INFO("Shader ", std::quoted(filePath), " successfully compiled");

	return true;
}

}
