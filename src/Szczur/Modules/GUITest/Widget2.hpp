#pragma once 

#include <vector>

#include <SFML/Graphics.hpp>

namespace rat
{
    class Widget2 : public sf::Drawable
    {
        using Children_t = std::vector<Widget2*>;
    public:
/*
        virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;
        void handleEvents(const sf::Event& event);
		void update(float deltaTime);

        void setSize(const sf::Vector2u& size); 
        void setSize(size_t x, size_t y);

		Widget* add(Widget* object);
        void setParent(Widget* parent);

		virtual void calculateSize();
		sf::Vector2u getSize() const;

		void setPosition(const sf::Vector2f& offset);
		void setPosition(float x, float y);
		void move(const sf::Vector2f& offset);
		void move(float offsetX, float offsetY);

		void activate();
		void deactivate();
		bool isActivated() const;

		void visible();
		void invisible();
		bool isVisible() const;

*/
        virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override;

        Widget2* add(Widget2* child);
        void setParent(Widget2* parent);

        void setPosition(float x, float y);
        void setMinimalSize(int x, int y);
        void setOrigin(float x, float y);
        void setOriginProp(float x, float y); 

        sf::Vector2i getSize() const;
        sf::Vector2f getPosition() const;

        virtual ~Widget2();

    private:

        Widget2* _parent{nullptr};
        Children_t _children;

        sf::FloatRect _bounds;
        sf::Vector2f _position;
        bool _isPosChanged{false};
        sf::Vector2f _origin;
        sf::Vector2f _originProp;
        bool _isOriginPropSet{false};
        sf::Vector2i _size;
        sf::Vector2i _minSize;
        bool _isMinSizeSet{false};

        void _recalcSize();
        void _recalcOrigin();
        void _recalcPos();

        virtual void _drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

        void _recalcChildrenPos();
        sf::Vector2f _getChildrenSize() const;

    /*
        bool _lives{true};

        sf::FloatRect _bounds;

        sf::Vector2u _minimalSize{0u, 0u};
        bool _isMinSizeSet{false};
        sf::Vector2u _size{0u, 0u};

        sf::Vector2i _origin{0, 0};
        sf::Vector2f _originProp{0.f, 0.f};
        bool _isOriginPropSet{false};

        Children_t _children;

        bool _isActivated;
		bool _isVisible;

        virtual void _recalcSize();
        sf::FloatRect _getGlobalBounds() const;

        sf::Vector2u _getChildrenSize() const;

        virtual void _draw(sf::RenderTarget& target, sf::RenderStates) const {}
        virtual void _drawChildren (sf::RenderTarget& target, sf::RenderStates states) const;
*/
    };
}