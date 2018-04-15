#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
namespace sf3d {

template<GLenum T>
	class Shader {
	public:
		Shader(const std::string& path) {
			std::ifstream file(path);
			if(file.is_open()) {
				std::string _code((std::istreambuf_iterator<char>(file)),
								  std::istreambuf_iterator<char>());
				const char* code = _code.c_str();
				_shader = glCreateShader(T);
				glShaderSource(_shader, 1, &code, NULL);
				glCompileShader(_shader);

				int  success;
				char infoLog[512];
				glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
				if(!success) {
					glGetShaderInfoLog(_shader, 512, NULL, infoLog);
					std::cout << "Compilation error: " << path << '\n' << infoLog << '\n';
					glDeleteShader(_shader);
				}
			}
			else
				std::cout << "Cannot open " << path << '\n';
		}
		~Shader() {
			glDeleteShader(_shader);
		}
		operator GLuint() {
			return _shader;
		}
	private:
		GLuint _shader;
	};

	using VShader = Shader<GL_VERTEX_SHADER>;
	using FShader = Shader<GL_FRAGMENT_SHADER>;
}