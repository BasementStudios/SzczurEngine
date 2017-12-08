#pragma once

#include <vector>
#include <functional>

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>


namespace rat {
	class Widget : public sf::Drawable, public sf::Transformable {
	public:
		Widget();
		Widget(const Widget&) = default;
		~Widget();
	public:
		enum class CallbackType {
			onHover, onHoverIn, onHoverOut, onPress, onHold, onRelease
		};

		using Function_t = std::function<void(Widget*)>;
		using CallbacksContainer_t = boost::container::flat_map<CallbackType, Function_t>;

		void setParent(Widget* parent);

		Widget* add(Widget* object);

		Widget* setCallback(CallbackType key, Function_t value);

		void clear();

		void input(const sf::Event& event);
		void update(float deltaTime);

		void calculateSize();
		sf::Vector2u getSize() const;

		void move(const sf::Vector2f& offset);
		void move(float offsetX, float offsetY);
		void setPosition(const sf::Vector2f& offset);
		void setPosition(float x, float y);

		void activate();
		void deactivate();
		bool isActivated() const;

		void visible();
		void invisible();
		bool isVisible() const;

	protected:
		virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const {}
		virtual void _update(float deltaTime) {}
		virtual void _input(const sf::Event& event) {}
		virtual sf::Vector2u _getSize() const;

		bool _aboutToRecalculate;
		
	private:
		std::vector<Widget*> _children;
		Widget* _parent;

		CallbacksContainer_t _callback;
		bool _isHovered;
		bool _isPressed;
		bool _isActivated;
		bool _isVisible;

		sf::Vector2u _size;

		void callback(CallbackType type);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	};
}