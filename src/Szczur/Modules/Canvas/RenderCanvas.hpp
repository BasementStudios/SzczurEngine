#pragma once

/** @file RenderCanvas.hpp
 ** @description Header file with RenderCanvas class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <array>						// array
#include <memory>						// unique_ptr

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

#include "RenderLayer.hpp"

namespace rat 
{

/** @class RenderCanvas
 ** @description Manages render layers.
 **/
template <std::size_t N>
class RenderCanvas
{
	/* Types */
public:
	using LayerID_t = std::size_t;
	using Container_t = std::array<std::unique_ptr<rat::RenderLayer>, N>;

	

	/* Fields */
protected:
	sf::RenderTarget*	_target = nullptr;
	sf::RenderStates	_states;
    sf::Vector2u        _size;
	Container_t 		_layers;



	/* Properties */
public:
	/** @property Target
	 ** @description Provides access to final RenderTarget. 
	 ** @access get set
	 **/
	sf::RenderTarget& getTarget();
	const sf::RenderTarget& getTarget() const;
	void setTarget(sf::RenderTarget& target);

	/** @property States
	 ** @description Provides access to final RenderTarget. 
	 ** @access get set
	 **/
	sf::RenderStates getStates() const;
	void setStates(const sf::RenderStates& states = sf::RenderStates::Default);

	/** @property Size
	 ** @description Size of the render layers. 
	 ** @access get set
	 ** @returns Vector2u
	 **/
	sf::Vector2u getSize() const;
	void setSize(const sf::Vector2u& size);
	void setSize(unsigned int width, unsigned int height);
	
	/** @property Layer
	 ** @description Provides access to specific RenderLayer. 
	 ** @access get
	 ** @argument LayerID_t LayerID - ID of layer to reference return. 
	 **/
	//template<LayerID_t LayerID>
	//rat::RenderLayer& getLayer();
	//template<LayerID_t LayerID>
	//const rat::RenderLayer& getLayer() const;
	rat::RenderLayer& getLayer(LayerID_t LayerID);
	const rat::RenderLayer& getLayer(LayerID_t LayerID) const;



    /* Operators */
public:
    RenderCanvas();
	
	RenderCanvas(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default);


	
	/* Methods */
public:
	/** @method display
     ** @description Renders the layers to the target. 
     **/
	void display();

	/** @method draw
	 ** @description Draw a drawable objects or primitives (by an array of vertices) to the render target.
	 **/
	//template<LayerID_t LayerID>
 	//void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) const;
	//template<LayerID_t LayerID>
  	//void draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default) const;
 	void draw(LayerID_t LayerID, const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) const;
  	void draw(LayerID_t LayerID, const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default) const;

private:
	/// resetLayers
	void resetLayers();
};

}

#include "RenderCanvas.tpp"
