#pragma once

#include "SFML/Graphics.hpp"

#include "Widget.hpp"

namespace rat {
    class Wedge : public sf::Shape {
    public:
        Wedge(float radius, float startRotate, float fraction, sf::Vector2f position = {0.f, 0.f});

        void update(float deltaTime);

        void fadeOut(float distance);

        void fadeIn();

        bool isColliding(sf::Vector2f mouse);

        virtual size_t getPointCount() const override;
        virtual sf::Vector2f getPoint(size_t index) const override;

    private:

        float _radius;
        float _startRotate;
        float _fraction;

        bool _isMoving;

        sf::Vector2f _fadeDirection;
        sf::Vector2f _basePosition;
        sf::Vector2f _targetPosition;
        
    };
}

namespace rat {
    class CircleChooseWidget : public Widget {
    public:
        using Wedges_t = std::vector<Wedge*>;
        CircleChooseWidget();
        ~CircleChooseWidget();

        void setAmount(size_t amount);

        void setRadius(float radius);

        void setGap(float gap);

        void setExtension(float extension);

        void setColor(sf::Color color);

    protected:
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		virtual void _update(float deltaTime) override;
		virtual void _input(const sf::Event& event) override;
        virtual sf::Vector2u _getSize() const override;
    private:
        void _recreate();

        Wedges_t _wedges;

        bool _toRecreate;

        size_t _amount;
        float _radius;
        float _gap;
        float _extension;
        sf::Color _color;
    };
}