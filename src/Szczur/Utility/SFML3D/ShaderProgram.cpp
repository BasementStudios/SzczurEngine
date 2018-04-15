#include "ShaderProgram.hpp"

namespace sf3d {
	ShaderProgram::ShaderProgram(VShader vShader, FShader fShader) {
		_program = glCreateProgram();
		glAttachShader(_program, vShader);
		glAttachShader(_program, fShader);
		glLinkProgram(_program);
		glDetachShader(_program, vShader);
		glDetachShader(_program, fShader);
	}
	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(_program);
	}
	void ShaderProgram::use() const {
		glUseProgram(_program);
	}
	ShaderProgram::operator GLuint() {
		return _program;
	}
}