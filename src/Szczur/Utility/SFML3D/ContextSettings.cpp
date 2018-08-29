#include "ContextSettings.hpp"

/** @file ContextSettings.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <stdexcept>

#include <glad/glad.h>

namespace sf3d
{

/* Variables */
const ContextSettings ContextSettings::None {0u, 0u, 0u, 0u};
const ContextSettings ContextSettings::Default {};



/* Properties */
GLuint ContextSettings::getColorInternalFormat() const
{
	return this->colorSpace; // @todo .
}

GLuint ContextSettings::getRenderInternalFormat() const
{
	switch (this->depthBits) {
		case 24:
		{
			switch (this->stencilBits) {
				case 8:
				{
					return GL_DEPTH24_STENCIL8;
				}
			}
		}
		// @todo . rest of it? 
	}
	throw std::logic_error(std::string("Unsupported context settings: {depthBits: ") + std::to_string(this->depthBits) + ", stencilBits: " + std::to_string(this->stencilBits) + "}");
	return 0;
}

GLuint ContextSettings::getRenderAttachmentType() const
{
	if (this->depthBits > 0) {
		if (this->stencilBits > 0) {
			return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		// @todo . rest of it ;F
	}
	throw std::logic_error("Not implemented yet.");
	return 0;
}

bool ContextSettings::isNone() const noexcept
{
	return this->multisamplingLevel == 0;
}


/* Operators */
bool ContextSettings::operator == (const ContextSettings& other) const noexcept
{
	if (
		this->depthBits == other.depthBits &&
		this->stencilBits == other.stencilBits &&
		this->multisamplingLevel == other.multisamplingLevel &&
		this->colorSpace == other.colorSpace
	) {
		return true;
	}
	else {
		return false;
	}
}

bool ContextSettings::operator != (const ContextSettings& other) const noexcept
{
	return !this->operator==(other);
}

}
