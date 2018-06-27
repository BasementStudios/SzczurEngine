#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <vector>

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"

#include "ProportionalDimes.hpp"
#include "Widget/FamilyTransform.hpp"

#define GUI_DEBUG 1

namespace rat 
{
	class InterfaceWidget;
	class TransformAnimationBase;

	class Widget : public sf::Drawable, protected gui::FamilyTransform
	{
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
		using Animation_t = std::unique_ptr<TransformAnimationBase>;
		using AnimationsContainer_t = std::vector<Animation_t>;

		void setParent(Widget* parent);
		void setInterface(const InterfaceWidget* interface);

		Widget* add(Widget* object);

		Widget* setCallback(CallbackType key, Function_t value);
		Widget* setLuaCallback(CallbackType key, SolFunction_t value);

		void clear();

		void update(float deltaTime);

		virtual void calculateSize();
		void invokeToCalculate();

		void input(const sf::Event& event);
		void invokeInput(const sf::Event& event);

		sf::Vector2i getSize() const;
		sf::Vector2i getMinimalSize() const;

		void move(const sf::Vector2i& offset);
		void move(int offsetX, int offsetY);
		void setPosition(const sf::Vector2i& offset);
		void setPosition(int x, int y);
		const sf::Vector2i& getPosition() const;
		const sf::Vector2i& getGlobalPosition() const;

		void setPropPosition(const sf::Vector2f& propPos);
		void setPropPosition(float propX, float propY);

		virtual void setPadding(const sf::Vector2i& padding);
		virtual void setPadding(int width, int height);
		sf::Vector2i getPadding() const;

		sf::Vector2i getInnerSize() const;

		void setColor(const sf::Color& color);
		void setColor(const sf::Color& color, float inTime);
		void resetColor();
		sf::Color getColor() const; 
		
		void setOrigin(const sf::Vector2i& origin);
		void setOrigin(int x, int y);
		sf::Vector2i getOrigin() const;

		void setPropOrigin(const sf::Vector2f& prop);
		void setPropOrigin(float x, float y);

		void setSize(sf::Vector2i size);
		void setSize(int width, int height);

		void setPropSize(const sf::Vector2f& propSize);
		void setPropSize(float widthProp, float widthHeight);
		void lockSize();

		void activate();
		void deactivate();
		bool isActivated() const;

		void visible();
		void invisible();
		bool isVisible() const;

		void fullyDeactivate();
		void fullyActivate();
		bool isFullyDeactivated() const;

		void makeChildrenPenetrable();
		void makeChildrenUnresizable();

        void invokeToCalcPropPosition();
		void forceToUpdatePropSize();
		void invokeToCalcPosition();

		void applyFamilyTrans(const sf::Vector2i& globalPos, const sf::Vector2i& drawPos);

		Widget* operator[](size_t index);
		const Widget* operator[](size_t index) const;

		size_t getChildrenAmount() const;

		static void setWinProp(sf::Vector2f prop);

	protected:
		virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const {}
		virtual void _update(float deltaTime) {}
		virtual void _input(const sf::Event& event) {}
		virtual sf::Vector2i _getSize() const;
		virtual void _calculateSize() {}
		virtual void _setColor(const sf::Color& color) {}
		virtual void _addWidget(Widget* widget) {}
		virtual void _clear() {}

		virtual void _recalcChildrenPos();
		virtual void _recalcPos() {}

		virtual sf::Vector2i _getInnerSize() const;

		virtual sf::Vector2i _getChildrenSize();
		virtual void _drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

		bool _onPressed();
		void _onRealesed();
		void _onMoved(const sf::Vector2i& mousePos);
		bool _aboutToRecalculate;
		bool _isPosChanged{false};

		bool _isHovered;
		bool _isPressed;
		bool _isActivated;
		bool _isVisible;

		bool _isFullyDeactivated{false};

		sf::Vector2i _size;

		Widget* _parent;

		sf::Vector2i _minSize;
		bool _isMinSizeSet{false};

		ProportionalDimes _props;

		sf::Vector2i _padding;

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

		AnimationsContainer_t _animations;
		size_t _currentAnimations{0};
		void _addAnimation(Animation_t animation);
		void _updateAnimations(float dt);

		sf::Color _color;

		bool _areChildrenPenetrable{false}; //lenny
		bool _areChildrenResizing{true};

		sf::Vector2i _getBound() const;

		void _updatePropSize();

		bool _propSizeMustBeenCalculatedViaInterface{false};

		void _updatePropPosition();


		const InterfaceWidget* _interface{nullptr};

		bool _childrenPropSizesMustBeenRecalculated{false};
		bool _propSizeMustBeenRecalculated{false};
		bool _propPosMustBeenRecalculated{false};


	protected:
		static sf::Vector2f _winProp;
	};
}
//#include "Widget.tpp"