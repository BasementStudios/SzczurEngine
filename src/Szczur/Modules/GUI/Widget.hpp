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
	namespace gui { class AnimBase; class AnimData; enum AnimType : int; }

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
		void setPositionInTime(const sf::Vector2f& offset, const gui::AnimData& data);
		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getGlobalPosition() const;

		void setPropPosition(const sf::Vector2f& propPos);
		void setPropPosition(float propX, float propY);
		void setPropPositionInTime(const sf::Vector2f& propPos, float inTime);
		void setPropPositionInTime(const sf::Vector2f& propPos, const gui::AnimData& data);

		sf::Vector2f getPosByGlobalPos(const sf::Vector2f& globalPos) const;
		void setGlobalPosition(const sf::Vector2f& globalPos);
		void setGlobalPosition(float globalX, float globalY);

		virtual void setPadding(const sf::Vector2f& padding);
		virtual void setPadding(float width, float height);
		sf::Vector2f getPadding() const;

		void setPropPadding(const sf::Vector2f& propPad);
		void setPropPadding(float propWidth, float propHeight);

		sf::Vector2f getInnerSize() const;

		void setColor(const sf::Color& color);
		void setColorInTime(const sf::Color& color, float inTime);
		void setColorInTime(const sf::Color& color, const gui::AnimData& data);
		void resetColor();
		sf::Color getColor() const; 
		
		void setOrigin(const sf::Vector2f& origin);
		void setOrigin(float x, float y);
		sf::Vector2f getOrigin() const;

		void setPropOrigin(const sf::Vector2f& prop);
		void setPropOrigin(float x, float y);

		void setSize(const sf::Vector2f& size);
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

		void makeUnresizable();
		void makePenetrable();

        void invokeToCalcPropPosition();
		void forceToUpdatePropSize();
		void invokeToCalcPosition();

		void applyFamilyTrans(const sf::Vector2f& globalPos, const sf::Vector2f& drawPos);

		Widget* operator[](size_t index);
		const Widget* operator[](size_t index) const;

		size_t getChildrenAmount() const;

		static void setWinProp(sf::Vector2f prop);

	protected:
		template<typename T>
		static void _initScript(ScriptClass<T>& object);

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
		virtual void _recalcElementsPropSize() {}

		virtual sf::Vector2f _getInnerSize() const;

		virtual sf::Vector2f _getChildrenSize();
		virtual void _drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

		void _addAnimation(Animation_t animation);
		void _abortAnimation(gui::AnimType type);

		Widget* _parent{nullptr};
		const InterfaceWidget* _interface{nullptr};

		bool _onPressed();
		void _onRealesed();
		void _onMoved(const sf::Vector2f& mousePos);
		bool _aboutToRecalculate{false};
		bool _isPosChanged{false};
		bool _elementsPropSizeMustBeenCalculated{false};

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
		bool _areChildrenResizable{true};

		bool _isPenetrable{false};
		bool _isResizable{true};

		sf::Vector2f _getBound() const;

		void _updatePropSize();

		bool _propSizeMustBeenCalculatedViaInterface{false};

		void _updatePropPosition();

		bool _childrenPropSizesMustBeenRecalculated{false};
		bool _propSizeMustBeenRecalculated{false};
		bool _propPosMustBeenRecalculated{false};

		bool _hasPropPadding{false};
		sf::Vector2f _propPadding;
		void _calcPropPadding();

	protected:
		static sf::Vector2f _winProp;
	};

	template<typename T>
	void Widget::_initScript(ScriptClass<T>& object)
	{
		object.setOverload("setPropSize",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setPropSize),
			sol::resolve<void(float, float)>(&Widget::setPropSize)
		);
		object.set("add", &Widget::add);
		object.set("clear", &Widget::clear);
		object.setOverload("move",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::move),
			sol::resolve<void(float, float)>(&Widget::move)
		);
		object.setOverload("setPosition",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setPosition),
			sol::resolve<void(float, float)>(&Widget::setPosition)
		);

		object.set("getPosition", &Widget::getPosition);
		object.set("getGlobalPosition", &Widget::getGlobalPosition);

		object.setOverload("setPropPosition",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setPropPosition),
			sol::resolve<void(float, float)>(&Widget::setPropPosition)
		);
		object.set("getPosByGlobalPos", &Widget::getPosByGlobalPos);
		object.setOverload("setGlobalPosition",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setGlobalPosition),
			sol::resolve<void(float, float)>(&Widget::setGlobalPosition)
		);
		object.setOverload("setPadding",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setPadding),
			sol::resolve<void(float, float)>(&Widget::setPadding)
		);
		object.set("getPadding", &Widget::getPadding);
		object.setOverload("setPropPadding",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setPropPadding),
			sol::resolve<void(float, float)>(&Widget::setPropPadding)
		);

		object.set("setColor", &Widget::setColor);
		object.setOverload("setOrigin",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setOrigin),
			sol::resolve<void(float, float)>(&Widget::setOrigin)
		);
		object.set("getOrigin", &Widget::getOrigin);

		object.setOverload("setPropOrigin",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setPropOrigin),
			sol::resolve<void(float, float)>(&Widget::setPropOrigin)
		);
		object.setOverload("setSize",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setSize),
			sol::resolve<void(float, float)>(&Widget::setSize)
		);
		object.setOverload("setPropSize",
			sol::resolve<void(const sf::Vector2f&)>(&Widget::setPropSize),
			sol::resolve<void(float, float)>(&Widget::setPropSize)
		);

		object.set("activate", &Widget::activate);
		object.set("deactivate", &Widget::deactivate);
		object.set("isActivated", &Widget::isActivated);

		object.set("visible", &Widget::visible);
		object.set("invisible", &Widget::invisible);
		object.set("isVisible", &Widget::isVisible);

		object.set("fullyDeactivate", &Widget::fullyDeactivate);
		object.set("fullyActivate", &Widget::fullyActivate);
		object.set("isFullyDeactivated", &Widget::isFullyDeactivated);

		object.set("makeChildrenPenetrable", &Widget::makeChildrenPenetrable);
		object.set("makeChildrenUnresizable", &Widget::makeChildrenUnresizable);

		object.set("makeUnresizable", &Widget::makeUnresizable);
		object.set("makePenetrable", &Widget::makePenetrable);

		object.set("getChildrenAmount", &Widget::getChildrenAmount);

		object.set(sol::meta_function::index, [&](Widget& obj, int index){ return obj[index]; });
		object.setProperty("onPress", 
			[](Widget& obj)->Widget&{ return obj; }, 
			[](Widget& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onPress, callback); }
		);
		object.setProperty("onRelease", 
			[](Widget& obj)->Widget&{ return obj; }, 
			[](Widget& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onRelease, callback); }
		);
		object.setProperty("onHover", 
			[](Widget& obj)->Widget&{ return obj; }, 
			[](Widget& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHover, callback); }
		);
		object.setProperty("onHoverIn", 
			[](Widget& obj)->Widget&{ return obj; }, 
			[](Widget& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHoverIn, callback); }
		);
		object.setProperty("onHoverOut", 
			[](Widget& obj)->Widget&{ return obj; }, 
			[](Widget& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHoverOut, callback); }
		);
		object.setProperty("onHold", 
			[](Widget& obj)->Widget&{ return obj; }, 
			[](Widget& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHold, callback); }
		);
		
		// object.setOverload(
		// 	sol::resolve<void(const sf::Vector2f&)>(&Widget::setPosition),
		// 	sol::resolve<void(float, float)>(&Widget::setPosition),
		// 	sol::resolve<void(const sf::Vector2f&, float)>(&Widget::setPosition),
		// 	sol::resolve<void(const sf::Vector2f&, const gui::AnimData&)>(&Widget::setPosition)
		// );
	}
}


//#include "Widget.tpp"