#pragma once

/** @file SpriteDisplayer.hpp
 ** @description Header file with SpriteDisplayer class as simplest of displayers.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Szczur/Modules/Assets/Asset.hpp"
#include "Szczur/Modules/World/SceneObject.hpp"

namespace rat
{

/** @class SpriteDisplayer
 ** @desciption Simplest, one sprite object displayer.
 ** @ TObject - type of object to be displayed.
 **/
template <typename TObject>
class SpriteDisplayer : public Displayer<TObject> // @todo TObject assertion for position/name...?
{
public:
    /** @method draw
     ** @description Draws the object on the render target with the render states.
     ** @virtual override
     **/
    virtual void draw(const TObject& object, sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
    
    virtual ~SpriteDisplayer() override {};
};



/** @class SpriteDisplayer
 ** @desciption Simplest, one sprite object displayer for SceneObject.
 ** @template specialization for SceneObject
 **/
template <>
class SpriteDisplayer<SceneObject> : public Displayer<SceneObject>
{
	/* Fields */
protected:
	// @todo ! ratAsset do użycia
    //rat::Asset<sf::Texture>*     _textureAsset   = nullptr;
    sf::Texture*    _texture         = nullptr;
    sf::Vector2f    _origin         {0, 0};
    sf::Vertex      _vertices[4];
    sf::IntRect     _textureRect; // @todo ? sumie jesłi ten rect byłby tu neizmienny można by to usunąć i odrazu wyliczać vertices



    /* Properties */
protected:
    /** @property TextureRect
     ** @description Defines fragment of the texture to use. Aslo updates vertices for that fragment coords.
     ** @access protected set
     **/
    void setTextureRect(const sf::IntRect rect);
	


	/* Operators */
public:
	SpriteDisplayer(Assets& assets, SceneObject& object);
	virtual ~SpriteDisplayer() override;



    /* Methods */
public:
	/** @method draw
     ** @description Draws the scene object.
     ** @virtual override
     **/
    virtual void draw(const SceneObject& object, sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

protected:
    /** @method updateVertices
     ** @description Updates vertices position and tex coords.
     **/
    void updateVertices();
};

}
