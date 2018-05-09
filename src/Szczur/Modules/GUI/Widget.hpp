#pragma once

#include <vector>
#include <functional>

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>


#include "Szczur/Modules/Script/Script.hpp"

//#define GUI_DEBUG 1

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
		void invokeToCalculate();

		sf::Vector2u getSize() const;
		sf::Vector2u getMinimalSize() const;

		void move(const sf::Vector2f& offset);
		void move(float offsetX, float offsetY);
		void setPosition(const sf::Vector2f& offset);
		void setPosition(float x, float y);

		virtual void setPadding(const sf::Vector2f& padding);
		virtual void setPadding(float width, float height);
		sf::Vector2f getPadding() const;

		void setOrigin(const sf::Vector2f& origin);
		void setOrigin(float x, float y);

		void setPropOrigin(const sf::Vector2f& prop);
		void setPropOrigin(float x, float y);

		void setSize(sf::Vector2u size);
		void setSize(size_t width, size_t height);
		void lockSize();

		void activate();
		void deactivate();
		bool isActivated() const;

		void visible();
		void invisible();
		bool isVisible() const;

		static void setWinProp(sf::Vector2f prop);

	protected:
		virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const {}
		virtual void _update(float deltaTime) {}
		virtual void _input(const sf::Event& event) {}
		virtual sf::Vector2u _getSize() const;
		virtual void _calculateSize() {}

		virtual void _inputChildren(sf::Event event);

		virtual sf::Vector2u _getChildrenSize();
		virtual void _drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

		bool _aboutToRecalculate;

		bool _isHovered;
		bool _isPressed;
		bool _isActivated;
		bool _isVisible;

		sf::Vector2u _size;

		Widget* _parent;

		sf::Vector2u _minSize;
		bool _isMinSizeSet{false};

		sf::Vector2f _origin{0.f, 0.f};
		sf::Vector2f _propOrigin;
		bool _isPropOriginSet{false};

		sf::Vector2f _padding;

		CallbacksContainer_t _callbacks;
		CallbacksLuaContainer_t _luaCallbacks;
		
		Children_t _children;
	private:
		virtual void _callback(CallbackType type);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		#ifdef GUI_DEBUG
		void _drawDebug(sf::RenderTarget& target, sf::RenderStates states) const;
		#endif

		void _recalcOrigin();

	protected:
		static sf::Vector2f _winProp;
	};
}
//#include "Widget.tpp"