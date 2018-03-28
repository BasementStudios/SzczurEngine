#pragma once

#include "Transform.hpp"
#include "SFML/Graphics/BlendMode.hpp"

namespace sf {
    class Texture;
    class BlendMode;
    class Shader;
}

namespace rat::sf3d {
    

    class RenderStates
    {
    public:
        RenderStates();
        RenderStates(const sf::BlendMode& theBlendMode);
        RenderStates(const Transform& theTransform);
        RenderStates(const sf::Texture* theTexture);
        RenderStates(const sf::Shader* theShader);
        RenderStates(const sf::BlendMode& theBlendMode, const Transform& theTransform,
                    const sf::Texture* theTexture, const sf::Shader* theShader);

        static const RenderStates Default; 

        sf::BlendMode       blendMode;
        Transform           transform; 
        const sf::Texture*  texture;   
        const sf::Shader*   shader;    
    };
}