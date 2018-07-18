#include "Texture.hpp"

#include <string>
#include <stdexcept>

#include <glad/glad.h>
#include <SFML/Graphics/Image.hpp>

namespace sf3d
{

/* Properties */
GLuint Texture::getID() const noexcept
{
	return this->textureID;
}

glm::uvec2 Texture::getSize() const noexcept
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

Texture::Texture(const char* path)
{
	this->loadFromFile(path);
}
Texture::Texture(const std::string& path)
{
	this->loadFromFile(path);
}



/* Methods */
void Texture::create(glm::uvec2 size)
{
	// Delete first, if it is recreation and size is diffrent
	if (this->textureID && (this->size != size)) {
		glDeleteTextures(1, &(this->textureID));
		glGenTextures(1, &(this->textureID));
	}

	// Setup the texture
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->size = size;
}

void Texture::loadFromFile(const char* path)
{
	// Load texture file
	sf::Image image;
	if (!image.loadFromFile(path)) {
		throw std::runtime_error(std::string("Cannot load texture from ") + path);
	}
	const glm::uvec2 size = {image.getSize().x, image.getSize().y};

	// Delete first, if it is recreation and size is diffrent
	if (this->textureID && (this->size != size)) {
		glDeleteTextures(1, &(this->textureID));
		glGenTextures(1, &(this->textureID));
	}
	
	// Load into graphics 
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		size.x, size.y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->size = size;
}
void Texture::loadFromFile(const std::string& path)
{
	this->loadFromFile(path.c_str());
}

void Texture::bind() const noexcept
{
	if (this->textureID) {
		glBindTexture(GL_TEXTURE_2D, this->textureID);
	}
}
void Texture::unbind() const noexcept
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
