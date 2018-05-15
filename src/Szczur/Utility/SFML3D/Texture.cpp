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

	Texture::Texture(Texture&& other) {
		this->_texture = other._texture; 
		other._texture = 0u;
	}
	Texture& Texture::operator = (Texture&& other) {
		this->_texture = other._texture;
		other._texture = 0u;
		return *this;
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
			glBindTexture(GL_TEXTURE_2D, 0);
			return true;
		}
		return false;
	}

	bool Texture::loadFromFile(const std::string& path)
	{
		return this->loadFromFile(path.c_str());
	}

	void Texture::bind() const {
		if(_texture)
			glBindTexture(GL_TEXTURE_2D, _texture);
	}

	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::create(const glm::vec2& size) {
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unbind();
		_size = size;
	}

	GLuint Texture::getID() const {
		return _texture;
	}

	const glm::uvec2& Texture::getSize() const {
		return _size;
	}
}