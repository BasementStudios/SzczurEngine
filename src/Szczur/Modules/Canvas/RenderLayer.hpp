#pragma once

/** @file RemderLayer.hpp
 ** @description Header file with RenderLayer class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace rat
{

/** @class RenderLayer
 ** @description Render layer class.
 **/
class RenderLayer
{
	/* Types */
public:
	using LayerID_t = std::size_t;
	


	/* Fields */
private:
	sf::RenderTexture _texture;
	sf::RenderStates _states;



	/* Operators */
public:
	RenderLayer() {}
	
	RenderLayer(const sf::Vector2u& size)
	{
		setSize(size);
	}

	// Disable coping
	RenderLayer(const RenderLayer&) = delete;
	RenderLayer& operator = (const RenderLayer&) = delete;
	
	// Disable moving
	RenderLayer(RenderLayer&&) = delete;
	RenderLayer& operator = (RenderLayer&&) = delete;



	/* Methods */
public:
	/** @property Size
	 ** @desciption 
	 ** @access get set
	 **/
	sf::Vector2u getSize() const;
	void setSize(const sf::Vector2u& size);

	/** @property RenderStates
	 ** @description Defines the states used for drawing to a RenderTarget. 
	 ** @access get set
	 **/
	const sf::RenderStates getRenderStates() const;
	void setRenderStates(const sf::RenderStates& states);

	/// draw
	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	
	/// draw
	void draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

	/// display
	void display(sf::RenderTarget& target);
};

}
