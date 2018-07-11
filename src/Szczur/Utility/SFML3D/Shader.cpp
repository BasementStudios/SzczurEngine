#include "Shader.hpp"

#include <fstream>
#include <memory>
#include <stdexcept>

std::unique_ptr<const char[]> getFileContents(const char* filePath)
{
	std::ifstream file{ filePath, std::ios_base::binary };

	if (file)
	{
		file.seekg(0, std::ios_base::end);

		auto size = file.tellg();

		std::unique_ptr<char[]> ptr;

		if (size > 0)
		{
			file.seekg(0, std::ios_base::beg);

			ptr.reset(new char[static_cast<size_t>(size) + 1]);

			file.read(ptr.get(), size);
			ptr[size] = '\0';
		}

		return ptr;
	}

	return nullptr;
}

namespace sf3d
{

Shader::Shader(Shader&& rhs) noexcept
	: _shader { rhs._shader }
{
	rhs._shader = 0;
}

Shader& Shader::operator = (Shader&& rhs) noexcept
{
	if (this != &rhs)
	{
		_destroy();

		_shader = rhs._shader;
		rhs._shader = 0;
	}

	return *this;
}

Shader::~Shader()
{
	_destroy();
}

// Loading from files
void Shader::loadFromFile(ShaderType type, const char* filePath)
{
	#ifdef EDITOR
	{
		_type = type;
		_filePath = filePath;
		_dataPtr = nullptr;
		_dataSize = 0;
	}
	#endif // EDITOR

	auto ptr = getFileContents(filePath);

	if (!ptr)
	{
		throw std::runtime_error(std::string("Cannot load shader from ") + filePath);
	}

	_compile(type, ptr.get(), -1);
}
void Shader::loadFromFile(ShaderType type, const std::string& filePath)
{
	this->loadFromFile(type, filePath.c_str());
}

// Loading from memory
void Shader::loadFromMemory(ShaderType type, const char* data, GLint size)
{
	#ifdef EDITOR
	{
		_type = type;
		_filePath.clear();
		_dataPtr = data;
		_dataSize = size;
	}
	#endif // EDITOR

	_compile(type, data, size);
}
void Shader::loadFromMemory(ShaderType type, const std::string& data)
{
	this->loadFromMemory(type, data.c_str(), -1);
}

bool Shader::isValid() const
{
	return _shader;
}

Shader::NativeHandle_t Shader::getNativeHandle() const
{
	return _shader;
}

#ifdef EDITOR

void Shader::_reload()
{
	if (!_filePath.empty())
	{
		loadFromFile(_type, _filePath);
	}
	else if (_dataPtr != nullptr)
	{
		loadFromMemory(_type, _dataPtr, _dataSize);
	}
}

#endif // EDITOR

void Shader::_destroy()
{
	if (_shader)
	{
		glDeleteShader(_shader);
	}
}

void Shader::_compile(ShaderType type, const char* data, GLint size)
{
	_destroy();

	_shader = glCreateShader(type);
	glShaderSource(_shader, 1, &data, &size);
	glCompileShader(_shader);

	GLint success;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);

	if (success != GL_TRUE)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(_shader, sizeof(infoLog), nullptr, infoLog);

		_destroy();

		_shader = 0;

		throw std::runtime_error(
			std::string(infoLog)
			#ifdef EDITOR
			+ "Cannot compile shader from " + _filePath + "\n"
			#endif
		);
	}
}

}
