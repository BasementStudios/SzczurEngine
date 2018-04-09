#include "Texture.hpp"

#include <SFML/Graphics.hpp>

namespace sf3d {
	Texture::Texture() {
		glGenTextures(1, &_texture);
	}

	Texture::~Texture() {
		if(_texture)
			glDeleteTextures(1, &_texture);
	}

	bool Texture::loadFromFile(const char * path) {
		if(_texture)
			glDeleteTextures(1, &_texture);

		sf::Image image;
		if(image.loadFromFile(path)) {
			auto size = image.getSize();
			_size.x = size.x;
			_size.y = size.y;
			
			glBindTexture(GL_TEXTURE_2D, _texture);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA,
				image.getSize().x, image.getSize().y,
				0,
				GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()
			);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glBindTexture(GL_TEXTURE_2D, NULL);
			return true;
		}
		return false;
	}

	void Texture::bind() const {
		if(_texture)
			glBindTexture(GL_TEXTURE_2D, _texture);
	}

	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	void Texture::create(const glm::vec2& size) {
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unbind();
		_size = size;
	}

	GLuint Texture::getID() const {
		return _texture;
	}

	glm::uvec2 Texture::getSize() const {
		return _size;
	}
}