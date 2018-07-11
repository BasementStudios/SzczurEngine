#pragma once

#include "Widget.hpp"
#include "ImageWidget.hpp"
#include "ListWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "WindowWidget.hpp"
#include "ScrollAreaWidget.hpp"

#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{
namespace gui
{
    class WidgetScripts
    {
    public:
        template<typename T>
        static void set(ScriptClass<T>& object)
        {
            object.setOverload("setPropSize",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setPropSize),
                static_cast<void (T::*)(float, float)>(&T::setPropSize)
            );
            object.set("clear", &T::clear);
            object.setOverload("move",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::move),
                static_cast<void (T::*)(float, float)>(&T::move)
            );

            // object.setOverload("setPosition",
            //     static_cast<void (T::*)(float, float)>(&T::setPosition),
            //     static_cast<void (T::*)(const sf::Vector2f&)>(&T::setPosition)
            // );
            object.setOverload("setPosition", 
                static_cast<void (T::*)(float, float)>( &T::setPosition),
                static_cast<void (T::*)(const sf::Vector2f&)>( &T::setPosition)
            );

            object.set("getPosition", &T::getPosition);
            object.set("getGlobalPosition", &T::getGlobalPosition);

            object.setOverload("setPropPosition",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setPropPosition),
                static_cast<void (T::*)(float, float)>(&T::setPropPosition)
            );
            object.set("getPosByGlobalPos", &T::getPosByGlobalPos);
            object.setOverload("setGlobalPosition",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setGlobalPosition),
                static_cast<void (T::*)(float, float)>(&T::setGlobalPosition)
            );
            object.setOverload("setPadding",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setPadding),
                static_cast<void (T::*)(float, float)>(&T::setPadding)
            );
            object.set("getPadding", &T::getPadding);
            object.setOverload("setPropPadding",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setPropPadding),
                static_cast<void (T::*)(float, float)>(&T::setPropPadding)
            );

            object.setOverload("setColor",
                static_cast<void (T::*)(const sf::Color&)>(&T::setColor),
                static_cast<void (T::*)(unsigned char, unsigned char, unsigned char, unsigned char)>(&T::setColor)
            );
            object.set("makeChildrenUncolorable", &T::makeChildrenUncolorable);
            object.setOverload("setOrigin",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setOrigin),
                static_cast<void (T::*)(float, float)>(&T::setOrigin)
            );
            object.set("getOrigin", &T::getOrigin);

            object.setOverload("setPropOrigin",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setPropOrigin),
                static_cast<void (T::*)(float, float)>(&T::setPropOrigin)
            );
            object.setOverload("setSize",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setSize),
                static_cast<void (T::*)(float, float)>(&T::setSize)
            );
            object.setOverload("setPropSize",
                static_cast<void (T::*)(const sf::Vector2f&)>(&T::setPropSize),
                static_cast<void (T::*)(float, float)>(&T::setPropSize)
            );

            object.set("activate", &T::activate);
            object.set("deactivate", &T::deactivate);
            object.set("isActivated", &T::isActivated);

            object.set("visible", &T::visible);
            object.set("invisible", &T::invisible);
            object.set("isVisible", &T::isVisible);

            object.set("fullyDeactivate", &T::fullyDeactivate);
            object.set("fullyActivate", &T::fullyActivate);
            object.set("isFullyDeactivated", &T::isFullyDeactivated);

            object.set("makeChildrenPenetrable", &T::makeChildrenPenetrable);
            object.set("makeChildrenUnresizable", &T::makeChildrenUnresizable);

            object.set("makeUnresizable", &T::makeUnresizable);
            object.set("makePenetrable", &T::makePenetrable);

            object.set("getChildrenAmount", &T::getChildrenAmount);

            object.set(sol::meta_function::index, [&](T& obj, int index){ return obj[index]; });
            object.setProperty("onPress", 
                [](T& obj)->T&{ return obj; }, 
                [](T& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onPress, callback); }
            );
            object.setProperty("onRelease", 
                [](T& obj)->T&{ return obj; }, 
                [](T& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onRelease, callback); }
            );
            object.setProperty("onHover", 
                [](T& obj)->T&{ return obj; }, 
                [](T& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHover, callback); }
            );
            object.setProperty("onHoverIn", 
                [](T& obj)->T&{ return obj; }, 
                [](T& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHoverIn, callback); }
            );
            object.setProperty("onHoverOut", 
                [](T& obj)->T&{ return obj; }, 
                [](T& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHoverOut, callback); }
            );
            object.setProperty("onHold", 
                [](T& obj)->T&{ return obj; }, 
                [](T& obj, sol::function callback){ obj.setLuaCallback(Widget::CallbackType::onHold, callback); }
            );

            object.set("addWidget", [](T& owner)->Widget&{
            	auto w = new Widget;
            	owner.add(w);
            	return *w;
            });
            object.set("addImage", [](T& owner)->ImageWidget&{
            	auto image = new ImageWidget;
            	owner.add(image);
            	return *image;
            });
            object.set("addList", [](T& owner)->ListWidget&{
            	auto list = new ListWidget;
            	owner.add(list);
            	return *list;
            });
            object.set("addText", [](T& owner)->TextWidget&{
            	auto text = new TextWidget;
            	owner.add(text);
            	return *text;
            });
            object.set("addTextArea", [](T& owner)->TextAreaWidget&{
            	auto ta = new TextAreaWidget;
            	owner.add(ta);
            	return *ta;
            });
            object.set("addWindow", [](T& owner)->WindowWidget&{
            	auto w = new WindowWidget;
            	owner.add(w);
            	return *w;
            });
            object.set("addScrollArea", [](T& owner)->ScrollAreaWidget&{
            	auto sa = new ScrollAreaWidget;
            	owner.add(sa);
            	return *sa;
            });



        }
    };
}
}