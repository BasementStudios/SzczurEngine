#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace rat {
	class Widget : public sf::Drawable, public sf::Transformable {
	public:
		Widget(Widget* parent = nullptr);
		~Widget();


		Widget* add(Widget* widget);

		sf::Vector2u getSize();

		void update(float deltaTime);
		void input(const sf::Event& event);

	private:
		
		Widget* _parent;
		std::vector<Widget*> _children;

		virtual bool _input(const sf::Event& event);
		virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void _update(float deltaTime);
		virtual sf::Vector2u _getSize();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}