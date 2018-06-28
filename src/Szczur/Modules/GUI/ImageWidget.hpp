#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class Script;
    class ImageWidget : public Widget
    {
    public:
        ImageWidget();
        ImageWidget(sf::Texture* texture);

        static void initScript(Script& script);

        void setTexture(sf::Texture* texture);

        const sf::Texture* getTexture() const;

        void setScale(const sf::Vector2f& scale);
    
    protected:
        virtual sf::Vector2f _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void _calculateSize() override;
        virtual void _setColor(const sf::Color& color) override;

        virtual void _recalcPos() override;
    private:
        sf::Sprite _sprite; 

        bool _hasTexture{false};
    };
}