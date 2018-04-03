#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace sf3d {
	class Shader {
	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);

		void use();

		void setBool(const std::string &name, bool value) const;

		void setInt(const std::string &name, int value) const;

		void setFloat(const std::string &name, float value) const;

	private:
		void checkCompileErrors(unsigned int shader, std::string type);
	};
}