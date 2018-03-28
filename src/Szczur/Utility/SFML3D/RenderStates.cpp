#include "RenderStates.hpp"

#include <SFML/Graphics.hpp>

namespace rat::sf3d {
    const RenderStates RenderStates::Default(BlendMode(
        sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add,
        sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add));


    ////////////////////////////////////////////////////////////
    RenderStates::RenderStates() :
    blendMode(sf::BlendAlpha),
    transform(),
    texture  (NULL),
    shader   (NULL)
    {
    }


    ////////////////////////////////////////////////////////////
    RenderStates::RenderStates(const Transform& theTransform) :
    blendMode(sf::BlendAlpha),
    transform(theTransform),
    texture  (NULL),
    shader   (NULL)
    {
    }


    ////////////////////////////////////////////////////////////
    RenderStates::RenderStates(const sf::BlendMode& theBlendMode) :
    blendMode(theBlendMode),
    transform(),
    texture  (NULL),
    shader   (NULL)
    {
    }


    ////////////////////////////////////////////////////////////
    RenderStates::RenderStates(const sf::Texture* theTexture) :
    blendMode(sf::BlendAlpha),
    transform(),
    texture  (theTexture),
    shader   (NULL)
    {
    }


    ////////////////////////////////////////////////////////////
    RenderStates::RenderStates(const sf::Shader* theShader) :
    blendMode(sf::BlendAlpha),
    transform(),
    texture  (NULL),
    shader   (theShader)
    {
    }


    ////////////////////////////////////////////////////////////
    RenderStates::RenderStates(const sf::BlendMode& theBlendMode, const Transform& theTransform,
                            const sf::Texture* theTexture, const sf::Shader* theShader) :
    blendMode(theBlendMode),
    transform(theTransform),
    texture  (theTexture),
    shader   (theShader)
    {
    }
}