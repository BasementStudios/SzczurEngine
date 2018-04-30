#pragma once

#include "Widget2.hpp"

namespace rat
{
    class ImageWidget2 : public Widget2
    {
    public:
        void setTexture(const sf::Texture* texture);
    protected:
        virtual void _recalculateComponentsPosition() override;
        virtual sf::Vector2f _getComponentsSize() const override;
        virtual void _recalculateComponentsSize() override;
        virtual void _drawComponents(sf::RenderTarget& target, sf::RenderStates) const override;
    private:
        sf::Sprite _sprite;
        const sf::Texture* _texture{nullptr};
        bool _isSizeFixed{false};
        sf::Vector2f _fixedSize{};

        void _setSpriteSize(sf::Vector2f size);
        void _reacalcSpriteSize();
    };
}