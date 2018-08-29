#pragma once

/** @file ContextSettings.hpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glad/glad.h>

namespace sf3d
{

/// Contains and provide rendering context settings
class ContextSettings
{
	/* Variables */
public:
	/// None, invaild state used to specify no changes.
	static const ContextSettings None;

	/// Default context settings 
	static const ContextSettings Default;

	GLuint depthBits = 24;
	GLuint stencilBits = 8;

	GLuint multisamplingLevel {1u};

	GLuint colorSpace {GL_RGBA};



	/* Properties */
public:
	
	GLuint getColorInternalFormat() const;

	GLuint getRenderInternalFormat() const;

	GLuint getRenderAttachmentType() const;
	
	// @todo . 

	bool isNone() const noexcept;



	/* Operators */
public:
	constexpr ContextSettings(const ContextSettings& other) = default; 
	// @todo 5

	bool operator == (const ContextSettings& other) const noexcept;
	bool operator != (const ContextSettings& other) const noexcept;
};

}
