#include "Texture.hpp"

#include <SFML/Graphics.hpp>

namespace sf3d {
	Texture::Texture() {
		glGenTextures(1, &_textureID);
	}

	Texture::~Texture() {
		if(_textureID)
			glDeleteTextures(1, &_textureID);
	}

	Texture::Texture(Texture&& other) {
		this->_textureID = other._textureID; 
		other._textureID = 0u;
	}
	Texture& Texture::operator = (Texture&& other) {
		this->_textureID = other._textureID;
		other._textureID = 0u;
		return *this;
	}

	bool Texture::loadFromFile(const char * path) {
		if(_textureID)
			glDeleteTextures(1, &_textureID);

		sf::Image image;
		if(image.loadFromFile(path)) {
			auto size = image.getSize();
			_size.x = size.x;
			_size.y = size.y;
			
			glBindTexture(GL_TEXTURE_2D, _textureID);
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

	bool Texture::loadFromFile(const std::string& path)
	{
		return this->loadFromFile(path.c_str());
	}

	void Texture::bind() const {
		if(_textureID)
			glBindTexture(GL_TEXTURE_2D, _textureID);
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
		return _textureID;
	}

	const glm::uvec2& Texture::getSize() const {
		return _size;
	}
}