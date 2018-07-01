#pragma once

namespace sf {
    class RenderTarget;
    class RenderStates;
    class Color;
}
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace rat {
    class Script;
}

#include "Widget.hpp"

namespace rat {
    class ImageWidget : public Widget
    {
    public:
        ImageWidget();
        ImageWidget(sf::Texture* texture);

        static void initScript(Script& script);

        void setTexture(sf::Texture* texture);

        const sf::Texture* getTexture() const;

        void removeTexture();

        void setScale(const sf::Vector2f& scale);

        void setTextureRect(const sf::IntRect& rect);
        void setPropTextureRect(const sf::FloatRect& propRect);
    
    protected:
        virtual sf::Vector2f _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void _calculateSize() override;
        virtual void _setColor(const sf::Color& color) override;

        virtual void _recalcPos() override;
    private:
        sf::Sprite _sprite;

        sf::FloatRect _propTexRect;
        bool _hasPropTexRect{false};

        void _calcPropTexRect();

        bool _hasTexture{false};
    };
}