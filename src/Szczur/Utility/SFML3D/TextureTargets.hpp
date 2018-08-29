#pragma once

/** @file TextureTargets.hpp
 ** @description File that provide information and types related to texture targets.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glad/glad.h>

namespace sf3d
{

/// Texture target, specifies the target to which the texture is bound. 
enum TextureTarget : GLuint
{
	Simple1D            = GL_TEXTURE_1D, 
	Simple2D            = GL_TEXTURE_2D, 
	Simple3D            = GL_TEXTURE_3D, 
	Array1D             = GL_TEXTURE_1D_ARRAY, 
	Array2D             = GL_TEXTURE_2D_ARRAY, 
	Rectangle           = GL_TEXTURE_RECTANGLE, 
	CubeMap             = GL_TEXTURE_CUBE_MAP, 
	Buffer              = GL_TEXTURE_BUFFER, 
	Multisample2D       = GL_TEXTURE_2D_MULTISAMPLE, 
	MultisampleArray2D  = GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 
	None = 0,
	
	// Default used in various situations
	Default = Simple2D
};

}
