#pragma once

#include <string>
#include <fstream>
#include <iomanip>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Szczur/Utility/Logger.hpp"

#include <glad.h>

namespace sf3d
{
	

	template<GLenum T>
	class Shader
	{
	public:
		Shader() = default;

		Shader(const Shader&) = delete;
		Shader& operator = (const Shader&) = delete;

		Shader(Shader&&) = delete;
		Shader& operator = (Shader&&) = delete;

		~Shader() {
			_destroy();
		}

		bool loadFromFile(const std::string& filePath) {
			return _compile(filePath);
		}

		operator GLuint () const {
			return _shader;
		}

	private:

		static bool getFileContent(const std::string& filePath, std::vector<char>& buffer)
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

		void _destroy() {
			if (_shader) {
				glDeleteShader(_shader);
				_shader = 0u;
			}		
		}

		bool _compile(const std::string& filePath){
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
			_shader = glCreateShader(T);
			glShaderSource(_shader, 1, &src, nullptr);
			glCompileShader(_shader);

			// Handle compilation result
			GLint success;
			glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
			if (success != GL_TRUE) {
				GLchar infoLog[512];
				glGetShaderInfoLog(_shader, sizeof(infoLog) * sizeof(GLchar), nullptr, infoLog);
				LOG_ERROR("Unable to compile shader ", std::quoted(filePath), '\n', infoLog);
				_destroy();
				return false;
			}

			LOG_INFO("Shader ", std::quoted(filePath), " successfully compiled");

			return true;
		}

		GLuint _shader{0u};

	};

	using VShader = Shader<GL_VERTEX_SHADER>;
	using FShader = Shader<GL_FRAGMENT_SHADER>;

	

}
