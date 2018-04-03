#pragma once

#include <glad\glad.h>
#include <glm/glm.hpp>

namespace sf3d {
	class Texture {
	public:
		Texture();

		~Texture();

		bool loadFromFile(const char* path);

		void bind() const;
		void unbind() const;

		void create(const glm::vec2& size);

		GLuint getID() const;

		glm::uvec2 getSize() const;
	private:
		GLuint _texture{0u};
		glm::uvec2 _size{0.f, 0.f};

	};
}