#pragma once

/** @file SpriteDisplayData.hpp
 ** @description Header file with sprite display data class as shared game sprite display data.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <array>
#include <vector>

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"

namespace rat
{

class SpriteDisplayData
{
	/* Constants */
protected:
	constexpr static auto	assetsFolderPath	= "Assets/Objects/";
	constexpr static auto	configFilePath		= "/sprite.json";
	constexpr static auto 	textureFilePath		= "/texture.png";
	

	
	/* Fields */
public: // @todo prot&prop
	std::string			name;
protected:
	sf3d::Texture 		texture;
	sf3d::VertexArray	verticles	{4};



	/* Properties */
public:
	/** @property Texture
	 ** @description Texture used to draw the object.
	 ** @access const get
	 **/
	const sf3d::Texture& getTexture() const;

	/** @property Vertices
	 ** @description Vertices used to render the texture as the object.
	 ** @access const get
	 **/
	const sf3d::VertexArray& getVertices() const;

	

	/* Operators */
public:
	/** @constructor
	 ** @description Loads sprite display data.
	 ** @argument name - name of the sprite display data to load.
	 **/
	SpriteDisplayData(const std::string& name);

	// Disable coping
	//SpriteDisplayData(const SpriteDisplayData&) = delete; // @warn ?
	//SpriteDisplayData& operator = (const SpriteDisplayData&) = delete;

	// Disable moving
	//SpriteDisplayData(SpriteDisplayData&&) = delete;
	//SpriteDisplayData& operator = (SpriteDisplayData&&) = delete;
};

}
