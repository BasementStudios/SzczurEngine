#pragma once 

#include <vector>

#include <SFML/Graphics.hpp>

namespace rat
{
    class Widget2 : public sf::Drawable
    {
        using Children_t = std::vector<Widget2*>;

    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;

        Widget2* add(Widget2* child);
        void setParent(Widget2* parent);

        void setPosition(float x, float y);
        void setMinimalSize(int x, int y);
        void setOrigin(float x, float y);
        void setOriginProp(float x, float y); 

        sf::Vector2i getSize() const;
        sf::Vector2i getMinimalSize() const;
        sf::Vector2f getPosition() const;
        sf::FloatRect getGlobalBounds() const;

        virtual ~Widget2();

    protected:
        virtual void _setComponentsPosition(sf::Vector2f position) {}
        virtual sf::Vector2f _getComponentsSize() const { return {}; }
        virtual void _setComponentsSize(sf::Vector2f size) {}
        virtual void _drawComponents(sf::RenderTarget& target, sf::RenderStates) const {}
        virtual void _updateComponents(float dt) {}
        virtual void _handleComponentsEvent(sf::Event e) {}

    private:
        Widget2* _parent{nullptr};
        Children_t _children;

        sf::FloatRect _bounds;
        sf::Vector2f _position;
        sf::Vector2f _origin;
        sf::Vector2f _originProp;
        bool _isOriginPropSet{false};
        sf::Vector2i _size;
        sf::Vector2i _minSize;
        bool _isMinSizeSet{false};

        void _recalcSize();
        void _recalcOrigin();
        void _recalcPos();
        void _recalcPosByParent();

        virtual void _drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

        void _recalcChildrenPos();
        sf::Vector2f _getChildrenSize() const;

    };
}