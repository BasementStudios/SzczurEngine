#pragma once

#include <string>

#include "Szczur/Modules/Script/Script.hpp"

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class ImageWidget : public Widget {
    public:
        ImageWidget();
        ImageWidget(sf::Texture* texture);

        static void initScript(Script& script);

        void setTexture(sf::Texture* texture);

        const sf::Texture* getTexture() const;
    
    protected:
        virtual sf::Vector2u _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        virtual void _callback(CallbackType type) override;
        sf::Sprite _sprite; 
    };
}