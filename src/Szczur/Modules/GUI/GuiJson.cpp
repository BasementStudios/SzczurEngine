#include "GuiJson.hpp"


namespace rat {
    sf::Vector2u GuiJson::_windowSize = {0u,0u};

    void GuiJson::initJson(const std::string& filePath, Widget* root, const sf::Vector2u& windowSize) {
        //std::cout << std::stoi("+-100") << '\n';
        GuiJson::_windowSize = windowSize;

        std::ifstream file(filePath);
        Json json;
        file >> json;
        file.close();

        _browseJsonObject(json, root);
    }

    template<typename T, typename F>
    void GuiJson::_createJsonValue(Json& json, Widget* parent, F valuesCall) {
        if( std::is_base_of<Widget, T>() ) {
            T *widget = new T;
            for(Json::iterator it = json.begin(); it != json.end(); ++it) {
                std::string key = it.key();
                if(!valuesCall(widget, it)) {
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

                    else if(
                        key == "onHover" || 
                        key == "onHoverIn" || 
                        key == "onHoverOut" || 
                        key == "onPress" || 
                        key == "onHold" || 
                        key == "onRelease"
                    ) {
                        Json obj = *it;
                        
                        widget->setCallback([](const std::string &key){
                            if(key == "onHover")
                                return Widget::CallbackType::onHover;
                            else if(key == "onHoverIn")
                                return Widget::CallbackType::onHoverIn;
                            else if(key == "onHoverOut")
                                return Widget::CallbackType::onHoverOut;
                            else if(key == "onPress")
                                return Widget::CallbackType::onPress;
                            else if(key == "onHold")
                                return Widget::CallbackType::onHold;
                            else
                                return Widget::CallbackType::onRelease;
                            
                        }(key),[obj, widget, valuesCall](Widget*){
                            Json temp = obj;
                            for(auto it = temp.begin(); it != temp.end(); ++it) {
                                std::string key = it.key();
                                if(!valuesCall(widget, it)) {
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
                                }
                            }
                        });
                    }


                    else {
                        if(it->is_object())
                            _browseJsonObject(*it, widget);
                    }
                }
            }
            
            parent->add(widget);
        }
    }

    void GuiJson::_browseJsonObject(Json& json, Widget *parent) {
        std::string typeName;
        if(auto it = json.find("type"); it != json.end())
            typeName = it->get<std::string>();
        else
            typeName = "widget";
        
        /*
        if(typeName == 'typename') {    
            _createJsonValue<'widget'>(json, parent, []('widget'* widget, Json::iterator it){
                std::string key = it.key();
                'Conditions'
                else
                    return false;
                return true;
            });
        }
        */
        
        if(typeName == "widget") {
            _createJsonValue<Widget>(json, parent, [](Widget*, Json::iterator it){
                return false;
            });
        }

        if(typeName == "text") {
            _createJsonValue<TextWidget>(json, parent, [](TextWidget* widget, Json::iterator it){
                std::string key = it.key();
                if(key == "caption") {
                    widget->setString( it->get<std::string>() );
                }
                else if(key == "font") {
                    widget->setFont( it->get<std::string>() );
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

        if(typeName == "image") {    
            _createJsonValue<ImageWidget>(json, parent, [](ImageWidget* widget, Json::iterator it){
                std::string key = it.key();
                if(key == "src") {
                    widget->setTexture(it->get<std::string>());
                }
                else
                    return false;
                return true;
            });
        }
        
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