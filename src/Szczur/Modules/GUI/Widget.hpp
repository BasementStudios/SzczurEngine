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
			onHover, onHoverIn, onHoverOut, onPress, onHeld, onRelease
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







	/*
	class Widget : public sf::Drawable, public sf::Transformable {
	public:
		Widget();
		~Widget();

		void setParent(Widget* parent);
		Widget* add(Widget* widget);

		sf::Vector2u getSize();
		sf::Vector2u getPrecountedSize();

		void update(float deltaTime);
		void input(const sf::Event& event);

	private:
		
		Widget* _parent;
		std::vector<Widget*> _children;

		sf::Vector2u _precountedSize;

		virtual bool _input(const sf::Event& event);
		virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void _update(float deltaTime);

		virtual sf::Vector2u _getSize();
		

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};*/