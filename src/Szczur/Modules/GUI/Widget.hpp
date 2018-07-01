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
#include "TransformAnimBasics/Anim.hpp"

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

		using Animation_t = std::unique_ptr<gui::AnimBase>;
		using AnimationsContainer_t = std::vector<Animation_t>;

		using PosAnim_t = gui::Anim<Widget, gui::AnimBase::Type::Pos, sf::Vector2f>;
		using ColorAnim_t = gui::Anim<Widget, gui::AnimBase::Type::Color, sf::Color>;

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

		sf::Vector2f getSize() const;
		sf::Vector2f getMinimalSize() const;

		void move(const sf::Vector2f& offset);
		void move(float offsetX, float offsetY);
		void setPosition(const sf::Vector2f& offset);
		void setPosition(float x, float y);
		void setPositionInTime(const sf::Vector2f& offset, float inTime);
		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getGlobalPosition() const;

		void setPropPosition(const sf::Vector2f& propPos);
		void setPropPosition(float propX, float propY);
		void setPropPosition(const sf::Vector2f& propPos, float inTime);

		sf::Vector2f getPosByGlobalPos(const sf::Vector2f& globalPos) const;
		void setGlobalPosition(const sf::Vector2f& globalPos);
		void setGlobalPosition(float globalX, float globalY);

		virtual void setPadding(const sf::Vector2f& padding);
		virtual void setPadding(float width, float height);
		sf::Vector2f getPadding() const;

		sf::Vector2f getInnerSize() const;

		void setColor(const sf::Color& color);
		void setColorInTime(const sf::Color& color, float inTime);
		void resetColor();
		sf::Color getColor() const; 
		
		void setOrigin(const sf::Vector2f& origin);
		void setOrigin(float x, float y);
		sf::Vector2f getOrigin() const;

		void setPropOrigin(const sf::Vector2f& prop);
		void setPropOrigin(float x, float y);

		void setSize(sf::Vector2f size);
		void setSize(float width, float height);

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

		void applyFamilyTrans(const sf::Vector2f& globalPos, const sf::Vector2f& drawPos);

		Widget* operator[](size_t index);
		const Widget* operator[](size_t index) const;

		size_t getChildrenAmount() const;

		static void setWinProp(sf::Vector2f prop);

	protected:
		virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const {}
		virtual void _update(float deltaTime) {}
		virtual void _input(const sf::Event& event) {}
		virtual sf::Vector2f _getSize() const;
		virtual void _calculateSize() {}
		virtual void _setColor(const sf::Color& color) {}
		virtual void _addWidget(Widget* widget) {}
		virtual void _clear() {}

		virtual void _recalcChildrenPos();
		virtual void _recalcPos() {}

		virtual sf::Vector2f _getInnerSize() const;

		virtual sf::Vector2f _getChildrenSize();
		virtual void _drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

		void _addAnimation(Animation_t animation);
		void _abortAnimation(gui::AnimBase::Type type);

		Widget* _parent{nullptr};

		bool _onPressed();
		void _onRealesed();
		void _onMoved(const sf::Vector2f& mousePos);
		bool _aboutToRecalculate;
		bool _isPosChanged{false};

		bool _isHovered;
		bool _isPressed;
		bool _isActivated;
		bool _isVisible;

		bool _isFullyDeactivated{false};

		sf::Vector2f _size;

		sf::Vector2f _minSize;
		bool _isMinSizeSet{false};

		ProportionalDimes _props;

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

		AnimationsContainer_t _animations;
		size_t _currentAnimations{0};
		void _updateAnimations(float dt);

		sf::Color _color;

		bool _areChildrenPenetrable{false}; //lenny
		bool _areChildrenResizing{true};

		sf::Vector2f _getBound() const;

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