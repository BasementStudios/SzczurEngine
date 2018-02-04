#pragma once

/** @file RenderCanvas.tpp
 ** @description Templates implementation file for RenderCanvas class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace rat
{

/* Properties */
/// Target
template<std::size_t N>
sf::RenderTarget& RenderCanvas<N>::getTarget()
{
	return *(this->_target);
}
template<std::size_t N>
const sf::RenderTarget& RenderCanvas<N>::getTarget() const
{
	return *(this->_target);
}
template<std::size_t N>
void RenderCanvas<N>::setTarget(sf::RenderTarget& target)
{
	this->_target = &target;
	this->setSize(target.getSize());
}

/// States
template<std::size_t N>
sf::RenderStates RenderCanvas<N>::getStates() const
{
	return this->_states;
}
template<std::size_t N>
void RenderCanvas<N>::setStates(const sf::RenderStates& states)
{
	this->_states = states;
}

/// Size
template<std::size_t N>
sf::Vector2u RenderCanvas<N>::getSize() const
{
	return this->_size;
}
template<std::size_t N>
void RenderCanvas<N>::setSize(const sf::Vector2u& size)
{
	this->_size = size;
	this->resetLayers();
}
template<std::size_t N>
void RenderCanvas<N>::setSize(unsigned int width, unsigned int height)
{
	this->_size.x = width;
	this->_size.y = height;
	this->resetLayers();
}

/// Layer
template<std::size_t N>
template<typename RenderCanvas<N>::LayerID_t LayerID>
rat::RenderLayer& RenderCanvas<N>::getLayer()
{
	return *(this->_layers[LayerID]);
}
template<std::size_t N>
template<typename RenderCanvas<N>::LayerID_t LayerID>
const rat::RenderLayer& RenderCanvas<N>::getLayer() const
{
	return *(this->_layers[LayerID]);
}
template<std::size_t N>
rat::RenderLayer& RenderCanvas<N>::getLayer(RenderCanvas<N>::LayerID_t LayerID)
{
	return *(this->_layers[(std::size_t)LayerID]);
}
template<std::size_t N>
const rat::RenderLayer& RenderCanvas<N>::getLayer(RenderCanvas<N>::LayerID_t LayerID) const
{
	return *(this->_layers[(std::size_t)LayerID]);
}



/* Operators */
/// Constructor
template<std::size_t N>
RenderCanvas<N>::RenderCanvas()
{
	;
}
template<std::size_t N>
RenderCanvas<N>::RenderCanvas(sf::RenderTarget& target, const sf::RenderStates& states)
{
	this->setTarget(target);
	this->setStates(states);
}



/* Methods */
/// render
template<std::size_t N>
void RenderCanvas<N>::render()
{
	for (auto& layer : this->_layers) 
	    layer->display(this->getTarget());
	// @todo , może to variadic?? :thiking: :D 
}

/// draw
template<std::size_t N>
template<typename RenderCanvas<N>::LayerID_t LayerID>
void RenderCanvas<N>::draw(const sf::Drawable& drawable, const sf::RenderStates& states) const
{
	this->_layers[(size_t)LayerID]->draw(drawable, states);
}
template<std::size_t N>
template<typename RenderCanvas<N>::LayerID_t LayerID>
void RenderCanvas<N>::draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states) const
{
	this->_layers[(size_t)LayerID]->draw(vertices, vertexCount, type, states); 
}

/// resetLayers
template<std::size_t N>
void RenderCanvas<N>::resetLayers()
{
	for(auto& layer : this->_layers) 
	 	layer.reset(new rat::RenderLayer(this->_size)); 
	// @todo , może to variadic?? :thiking: :D 
}

}
