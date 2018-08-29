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
	return this->ID;
}

TextureTarget Texture::getTarget() const noexcept
{
	return this->target;
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
	this->destroy();
}

Texture::Texture(Texture&& other)
{
	if (this != &other) {
		this->ID = other.ID; 
		this->size = other.size;
		other.ID = 0u;
	}
}
Texture& Texture::operator = (Texture&& other)
{
	if (this != &other) {
		this->destroy();

		this->ID = other.ID; 
		this->size = other.size;
		other.ID = 0u;
	}
	return *this;
}

Texture::Texture(glm::uvec2 size, const ContextSettings& settings, TextureTarget target) 
{
	this->create(size, settings, target);
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
void Texture::create(glm::uvec2 size, const ContextSettings& settings, TextureTarget target)
{
	// Delete first, if it is recreation and size is diffrent
	if (this->size != size || this->target != target) {
		glDeleteTextures(1, &(this->ID));
		glGenTextures(1, &(this->ID));
	}
	else {
		// @todo . clear?
	}

	this->size = size;
	this->target = target;

	// Setup the texture
	switch (target) {
		case TextureTarget::Simple2D:
		{
			this->bind();
			glTexImage2D(GL_TEXTURE_2D, 0, settings.getColorInternalFormat(), size.x, size.y, 0, settings.getColorInternalFormat(), GL_UNSIGNED_BYTE, 0); // @todo ? Memory efficient textures - internal format =/= format?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			this->unbind();
		}
		break;

		case TextureTarget::Multisample2D:
		{
			if (settings.multisamplingLevel == 0) {
				throw std::logic_error("Invaild multisampling level: 0");
			}
			
			this->bind();
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, settings.multisamplingLevel, settings.getColorInternalFormat(), size.x, size.y, GL_TRUE);
			this->unbind();
		}
		break;

		default: 
		{
			throw std::logic_error(std::string("Unsupported texture target: ") + std::to_string(static_cast<GLuint>(target)));
		}
		break;
	}
}

void Texture::destroy()
{
	// `glDeleteTextures` silently ignores 0's and names that do not correspond to existing textures.
	glDeleteTextures(1, &(this->ID));
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
	if (this->size != size || this->target != TextureTarget::Simple2D) {
		glDeleteTextures(1, &(this->ID));
		glGenTextures(1, &(this->ID));
	}

	this->size = size;
	this->target = TextureTarget::Simple2D;
	
	// Load into graphics 
	this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()); // @todo ? Memory efficient textures 
	glGenerateMipmap(GL_TEXTURE_2D); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	this->unbind();
}
void Texture::loadFromFile(const std::string& path)
{
	this->loadFromFile(path.c_str());
}

void Texture::bind() const noexcept
{
	if (this->ID) {
		glBindTexture(static_cast<GLuint>(this->target), this->ID);
	}
}
void Texture::unbind() const noexcept
{
	glBindTexture(static_cast<GLuint>(this->target), 0);
}

}
