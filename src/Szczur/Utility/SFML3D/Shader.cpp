#include "Shader.hpp"

namespace sf3d {
	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::use() {
		glUseProgram(ID);
	}

	void Shader::setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void Shader::setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::checkCompileErrors(unsigned int shader, std::string type) {
		int success;
		char infoLog[1024];
		if(type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if(!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}