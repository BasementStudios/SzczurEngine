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

    void GuiJson::_browseJsonObject(Json& json, Widget *parent) {

        std::string typeName;
        if(auto it = json.find("type"); it != json.end())
            typeName = it->get<std::string>();
        else
            typeName = "widget";


//Add that kind of code, to add more variants of value

        if(typeName == "widget") {
            Widget* widget = new Widget;
            for(Json::iterator it = json.begin(); it != json.end(); ++it) {
                if(!_handleBasicValues(it, widget)) {
                    _browseJsonObject(*it, widget);
                }
            }
            parent->add(widget);
        }

        else if(typeName == "text") {
            TextWidget* widget = new TextWidget;
            for(Json::iterator it = json.begin(); it != json.end(); ++it) {
                if(!_handleBasicValues(it, widget)) {
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
                    else {
                        _browseJsonObject(*it, widget);
                    }
                }
            }
            parent->add(widget);
        }
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

        else if(key == "type") {

        }
        else
            return false;
        return true;
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