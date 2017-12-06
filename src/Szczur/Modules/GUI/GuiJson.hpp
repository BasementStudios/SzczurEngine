#pragma once

#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <boost/lexical_cast.hpp>

#include <Szczur/Json.hpp>

#include "Widget.hpp"
#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "InputWidget.hpp"
#include "CheckWidget.hpp"
#include "RadioWidget.hpp"



namespace rat {
    class GuiJson {
    public:
        static void initJson(const std::string& filePath, Widget* root, const sf::Vector2u& windowSize);
    private:
        static void _browseJsonObject(Json &json, Widget *parent);

        template<typename T>
        static void _createJsonValue(Json& json, Widget* parent, std::function<bool(T*, Json::iterator it)> call);

        static bool _handleBasicValues(Json::iterator it, Widget *widget);

        static int _stringToValue(const std::string &strOrigin, unsigned int valueOf);

        static sf::Vector2u _windowSize;
    };
}