#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace sf3d {
	class Texture {
	public:
		Texture();

		~Texture();

		Texture(Texture&& other);
		Texture& operator = (Texture&& other);

		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

		bool loadFromFile(const char* path);
		bool loadFromFile(const std::string& path);

		void bind() const;
		void unbind() const;

		void create(const glm::vec2& size);

		GLuint getID() const;

		const glm::uvec2& getSize() const;

	private:
		GLuint _textureID{0u};
		glm::uvec2 _size{0.f, 0.f};

	};
}