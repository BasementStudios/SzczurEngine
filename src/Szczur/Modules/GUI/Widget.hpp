#pragma once

#include <vector>
#include <functional>

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>


namespace rat {
	class Widget : public sf::Drawable, public sf::Transformable {
	public:
		Widget();
	public:
		enum class CallbackType {
			onHover, onHoverIn, onHoverOut, onPress, onHold, onRelease
		};

		using Function_t = std::function<void(Widget*)>;
		using CallbacksContainer_t = boost::container::flat_map<CallbackType, Function_t>;

		Widget* add(Widget* object);

		Widget* setCallback(CallbackType key, Function_t value);

		void input(const sf::Event& event);
		void update(float deltaTime);

		sf::Vector2u getSize() const;
		
	private:
		std::vector<Widget*> _children;

		CallbacksContainer_t _callback;
		bool _isHovered;
		bool _isPressed;

		void callback(CallbackType type);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	};
}