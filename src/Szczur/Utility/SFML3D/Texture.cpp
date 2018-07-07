#include "Texture.hpp"

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <SFML/Graphics/Image.hpp>

namespace sf3d
{

/* Properties */
GLuint Texture::getID() const
{
	return this->textureID;
}

glm::uvec2 Texture::getSize() const
{
	return this->size;
}



/* Operators */
Texture::Texture()
{
	;
}
Texture::~Texture()
{
	if (this->textureID) {
		glDeleteTextures(1, &(this->textureID));
	}
}

Texture::Texture(Texture&& other)
{
	if (this != &other) {
		this->textureID = other.textureID; 
		this->size      = other.size;
		other.textureID = 0u;
	}
}
Texture& Texture::operator = (Texture&& other)
{
	if (this != &other) {
		this->textureID = other.textureID; 
		this->size      = other.size;
		other.textureID = 0u;
	}
	return *this;
}

Texture::Texture(glm::uvec2 size) 
{
	this->create(size);
}



/* Methods */
void Texture::create(glm::vec2 size)
{
	// Delete first, if it is recreation
	if (this->textureID) {
		glDeleteTextures(1, &(this->textureID));
	}
	glGenTextures(1, &(this->textureID));

	// Setup the texture
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unbind();

	this->size = size;
}

bool Texture::loadFromFile(const char* path)
{
	if (this->textureID) {
		glDeleteTextures(1, &(this->textureID));
	}

	sf::Image image;
	if (image.loadFromFile(path)) {
		auto size = image.getSize();
		this->size.x = size.x;
		this->size.y = size.y;
		
		glBindTexture(GL_TEXTURE_2D, this->textureID);
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

void Texture::bind() const
{
	if (this->textureID) {
		glBindTexture(GL_TEXTURE_2D, this->textureID);
	}
}
void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
