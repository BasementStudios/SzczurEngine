#include "GuiJson.hpp"


namespace rat {
    void GuiJson::init(Json* json, BasicGuiAssetsManager* assets, Widget* root, const sf::Vector2u& windowSize) {
        _windowSize = windowSize;
        _assets = assets;
        _json = json;
        _browseJsonObject(*_json, root);
    }

    void GuiJson::reload(const sf::Vector2u newWindowSize, Widget* root) {
        root->clear();
        _windowSize = newWindowSize;
        _browseJsonObject(*_json, root);
    }

    void GuiJson::changeJson(Json* json, Widget* root) {
        _json = json;
        reload(_windowSize, root);
    }

    template<typename T, typename F>
    void GuiJson::_createJsonValue(Json& json, Widget* parent, F valuesCall) {
        static_assert( std::is_base_of<Widget, T>(), "Error in GuiJson::_createJsonValue, wrong value in template" );

        T *widget = new T;
        for(Json::iterator it = json.begin(); it != json.end(); ++it) {
            std::string key = it.key();
            if(!std::invoke(valuesCall, widget, it)) {
                if(!_handleBasicValues(it, widget)) {
                    if(
                        key == "onHover" || 
                        key == "onHoverIn" || 
                        key == "onHoverOut" || 
                        key == "onPress" || 
                        key == "onHold" || 
                        key == "onRelease"
                    ) {
                        Json &obj = *it;
                        widget->setCallback(
                            _stringTypeToEnum(key),
                            [this, &obj, widget, valuesCall](Widget*){
                                for(auto it = obj.begin(); it != obj.end(); ++it) {
                                    if(!std::invoke(valuesCall, widget, it)) {
                                        _handleBasicValues(it, widget);
                                    }
                                }
                            }
                        );
                    }
                    else {
                        if(it->is_object())
                            _browseJsonObject(*it, widget);
                    } 
                }
            }
        }
        
        parent->add(widget);
    }

    

    bool GuiJson::_handleBasicValues(Json::iterator it, Widget *widget) {
        std::string key = it.key();
        if(key == "visible") {
            if(it->get<bool>())
                widget->visible();
            else
                widget->invisible();
        }

        else if(key == "active") {
            if(it->get<bool>())
                widget->activate();
            else
                widget->deactivate();
        }

        else if(key == "position") {
            widget->setPosition(
                _stringToValue( (*it)[0].get<std::string>(), _windowSize.x),
                _stringToValue( (*it)[1].get<std::string>(), _windowSize.y)
            );
        }

        else
            return false;
        return true;   
    }

    void GuiJson::_browseJsonObject(Json& json, Widget *parent) {
        std::string typeName;
        if(auto it = json.find("type"); it != json.end())
            typeName = it->get<std::string>();
        else
            typeName = "widget";
        



        if(typeName == "widget") {
            _createJsonValue<Widget>(json, parent, [](Widget*, Json::iterator it){
                return false;
            });
        }

        else if(typeName == "text") {
            _createJsonValue<TextWidget>(json, parent, [this](TextWidget* widget, Json::iterator it){
                std::string key = it.key();
                if(key == "caption") {
                    widget->setString( it->get<std::string>() );
                }
                else if(key == "font") {
                    widget->setFont(_assets->get<sf::Font>(it->get<std::string>()));
                }
                else if(key == "color") {
                    widget->setColor(
                        sf::Color(
                            (*it)[0].get<unsigned int>(),
                            (*it)[1].get<unsigned int>(),
                            (*it)[2].get<unsigned int>()
                        )
                    );
                }
                else if(key == "fontsize") {
                    widget->setCharacterSize(it->get<unsigned int>());
                }
                else
                    return false;
                return true;
            });
        }

        else if(typeName == "image") {    
            _createJsonValue<ImageWidget>(json, parent, [this](ImageWidget* widget, Json::iterator it){
                std::string key = it.key();
                if(key == "src") {
                    widget->setTexture(_assets->get<sf::Texture>(it->get<std::string>()));
                }
                else
                    return false;
                return true;
            });
        }

        else if(typeName == "input") {    
            _createJsonValue<InputWidget>(json, parent, [this](InputWidget* widget, Json::iterator it){
                std::string key = it.key();
                if(key == "font") {
                    widget->setFont(_assets->get<sf::Font>(it->get<std::string>()));
                }
                else if(key == "color") {
                    widget->setColor(
                        sf::Color(
                            (*it)[0].get<unsigned int>(),
                            (*it)[1].get<unsigned int>(),
                            (*it)[2].get<unsigned int>()
                        )
                    );
                }
                else if(key == "fontsize") {
                    widget->setCharacterSize(it->get<unsigned int>());
                }
                else if(key == "caption") {
                    widget->setString( it->get<std::string>() );
                }
                else if(key == "bgcolor") {
                    widget->setBackgroundColor(
                        sf::Color(
                            (*it)[0].get<unsigned int>(),
                            (*it)[1].get<unsigned int>(),
                            (*it)[2].get<unsigned int>()
                        )
                    );
                }
                else if(key == "maxlength") {
                    widget->setMaxLength( it->get<unsigned int>() );
                }
                else
                    return false;
                return true;
            });
        }

        /*else if(typeName == "check") {    
            _createJsonValue<CheckWidget>(json, parent, [this](CheckWidget* widget, Json::iterator it){
                std::string key = it.key();
                if(key == "") {
                    
                }
                else
                    return false;
                return true;
            });
        }
               
        /*
        else if(typeName == 'typename') {    
            _createJsonValue<'widget'>(json, parent, [this]('widget'* widget, Json::iterator it){
                std::string key = it.key();
                'Conditions'
                else
                    return false;
                return true;
            });
        }
        */
        
    }

    Widget::CallbackType GuiJson::_stringTypeToEnum(const std::string &name) {
        if(name == "onHover")
            return Widget::CallbackType::onHover;
        else if(name == "onHoverIn")
            return Widget::CallbackType::onHoverIn;
        else if(name == "onHoverOut")
            return Widget::CallbackType::onHoverOut;
        else if(name == "onPress")
            return Widget::CallbackType::onPress;
        else if(name == "onHold")
            return Widget::CallbackType::onHold;
        else
            return Widget::CallbackType::onRelease;
    }

    int GuiJson::_stringToValue(const std::string &strOrigin, unsigned int valueOf) {
        std::string str = strOrigin;
        for(auto it = str.begin(); it != str.end(); ++it) {
            if(*it == ' ') {
                str.erase(it);
                --it;
            }
        }
        int value1 = std::stoi(str);
        int value2 = 0;
        str.erase(0, std::to_string(value1).length());
        if(str.length() > 0) {
            if(str[0] == '%')
                value1 = value1 * valueOf / 100;
        }
        str.erase(0,1);
        if(str.length() > 0)
            value2 = std::stoi(str);
        str.erase(0, std::to_string(value2).length());
        if(value2 > 0)
            str.erase(0,1);
        if(str.length() > 0) {
    
            if(str[0] == '%')
                value2 = value2 * valueOf / 100;
        }
        return value1 + value2;
    }
}