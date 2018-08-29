#pragma once

/** @file Texture.hpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski 
 **/

#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "TextureTargets.hpp"
#include "ContextSettings.hpp"

namespace sf3d
{

class Texture
{
	/* Variables */
protected:
	GLuint ID {0u};
	TextureTarget target {0u};
	glm::uvec2 size {0u, 0u};

	

	/* Properties */
public:
	/// ID that identify texture by graphic card.
	GLuint getID() const noexcept;

	/// Specifies the target to which the texture is bound. 
	TextureTarget getTarget() const noexcept;

	/// Size of the texture.
	glm::uvec2 getSize() const noexcept;

	

	/* Operators */
public:
	Texture();
	~Texture();

	Texture(Texture&& other);
	Texture& operator = (Texture&& other);

	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;
	
	Texture(glm::uvec2 size, const ContextSettings& settings = ContextSettings::Default, TextureTarget target = TextureTarget::Default);
	
	/// Constructs texture by loading from file
	Texture(const char* path);
	Texture(const std::string& path);



	/* Methods */
public:
	void create(glm::uvec2 size, const ContextSettings& settings = ContextSettings::Default, TextureTarget target = TextureTarget::Default);

	void destroy();

	void loadFromFile(const char* path);
	void loadFromFile(const std::string& path);

	void bind() const noexcept;
	void unbind() const noexcept;
};

}
