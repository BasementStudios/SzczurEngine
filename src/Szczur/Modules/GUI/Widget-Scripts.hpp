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
                sol::resolve<void(const sf::Vector2f&)>(&Widget::setPropSize),
                sol::resolve<void(float, float)>(&Widget::setPropSize)
            );
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

            object.set("addWidget", [](Widget& owner)->Widget&{
            	auto w = new Widget;
            	owner.add(w);
            	return *w;
            });
            object.set("addImage", [](Widget& owner)->ImageWidget&{
            	auto image = new ImageWidget;
            	owner.add(image);
            	return *image;
            });
            object.set("addList", [](Widget& owner)->ListWidget&{
            	auto list = new ListWidget;
            	owner.add(list);
            	return *list;
            });
            object.set("addText", [](Widget& owner)->TextWidget&{
            	auto text = new TextWidget;
            	owner.add(text);
            	return *text;
            });
            object.set("addTextArea", [](Widget& owner)->TextAreaWidget&{
            	auto ta = new TextAreaWidget;
            	owner.add(ta);
            	return *ta;
            });
            object.set("addWindow", [](Widget& owner)->WindowWidget&{
            	auto w = new WindowWidget;
            	owner.add(w);
            	return *w;
            });
            object.set("addScrollArea", [](Widget& owner)->ScrollAreaWidget&{
            	auto sa = new ScrollAreaWidget;
            	owner.add(sa);
            	return *sa;
            });



        }
    };
}
}