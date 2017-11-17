#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace rat {
	class Widget : public sf::Drawable, public sf::Transformable {
	public:
		Widget(Widget* parent = nullptr);
		~Widget();


		Widget* add(Widget* widget);

		virtual void update(float deltaTime);

	private:
		
		Widget* _parent;
		std::vector<Widget*> _children;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}