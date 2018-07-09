#pragma once

#include <vector>
#include <functional>

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>


#include "Szczur/Modules/Script/Script.hpp"



namespace rat {
	class Widget : public sf::Drawable, public sf::Transformable {
	public:
		Widget();
		Widget(const Widget&) = default;
		~Widget();

		static void initScript(Script& script);

		
	public:
		enum class CallbackType {
			onHover, onHoverIn, onHoverOut, onPress, onHold, onRelease
		};

		using SolFunction_t = sol::function;
		using Function_t = std::function<void(Widget*)>;
		using CallbacksContainer_t = boost::container::flat_map<CallbackType, Function_t>;
		using CallbacksLuaContainer_t = boost::container::flat_map<CallbackType, SolFunction_t>;
		using Children_t = std::vector<Widget*>;

		void setParent(Widget* parent);

		Widget* add(Widget* object);

		Widget* setCallback(CallbackType key, Function_t value);
		Widget* setLuaCallback(CallbackType key, SolFunction_t value);

		void clear();

		void input(sf::Event event);
		void update(float deltaTime);

		virtual void calculateSize();
		sf::Vector2u getSize() const;
		sf::Vector2u getMinimalSize() const;

		void move(const sf::Vector2f& offset);
		void move(float offsetX, float offsetY);
		void setPosition(const sf::Vector2f& offset);
		void setPosition(float x, float y);

		void setOrigin(const sf::Vector2f& origin);
		void setOrigin(float x, float y);

		void setPropOrigin(const sf::Vector2f& prop);
		void setPropOrigin(float x, float y);

		void setSize(sf::Vector2u size);
		void setSize(size_t width, size_t height);

		void activate();
		void deactivate();
		bool isActivated() const;

		void visible();
		void invisible();
		bool isVisible() const;

		static void setWinProp(sf::Vector2f prop);

	protected:
		virtual void _draw([[maybe_unused]] sf::RenderTarget& target, [[maybe_unused]] sf::RenderStates states) const {}
		virtual void _update([[maybe_unused]] float deltaTime) {}
		virtual void _input([[maybe_unused]] const sf::Event& event) {}
		virtual sf::Vector2u _getSize() const;
		virtual void _calculateSize() {}

		Widget* _parent;

		bool _isHovered;
		bool _isPressed;
		bool _isActivated;
		bool _isVisible;

		bool _aboutToRecalculate;

		sf::Vector2u _size;
		sf::Vector2u _minSize;

		bool _isMinSizeSet{false};

		sf::Vector2f _origin{0.f, 0.f};
		sf::Vector2f _propOrigin;
		bool _isPropOriginSet{false};

		CallbacksContainer_t _callbacks;
		CallbacksLuaContainer_t _luaCallbacks;
		
		Children_t _children;
	private:
		virtual void _callback(CallbackType type);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void _recalcOrigin();

		static sf::Vector2f _winProp;


	};
}
//#include "Widget.tpp"