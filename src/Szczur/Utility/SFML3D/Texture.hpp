#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace sf3d
{

class Texture
{
	/* Variables */
protected:
	GLuint textureID {0u};
	glm::uvec2 size {0u, 0u};

	

	/* Properties */
public:
	/// ID that identify texture by graphic card
	GLuint getID() const noexcept;

	/// Size of the texture
	glm::uvec2 getSize() const noexcept;

	

	/* Operators */
public:
	Texture();
	~Texture();

	Texture(Texture&& other);
	Texture& operator = (Texture&& other);

	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;
	
	Texture(glm::uvec2 size);
	
	/// Constructs texture by loading from file
	Texture(const char* path);
	Texture(const std::string& path);



	/* Methods */
public:
	void create(glm::vec2 size);

	void loadFromFile(const char* path);
	void loadFromFile(const std::string& path);

	void bind() const noexcept;
	void unbind() const noexcept;
};

}
