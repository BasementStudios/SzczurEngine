#pragma once

#include "Szczur/Modules/GUI/GUI.hpp"

#include "TextManager.hpp"

#include "Options.hpp"

namespace rat {
    class DialogGUI {
    public:
        DialogGUI(GUI& gui);

        ~DialogGUI();

        void create();

        void clear();

        void interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t)> callback);

        void setText(const std::string& text);

        void setCharacter(const std::string& name);

        void setCharacterTexture(sf::Texture* texture);
    private:
        GUI& _gui;
        Interface* _interface;
        TextAreaWidget* _area;
        TextWidget* _name;
        ImageWidget* _characterTexture;
        Widget* _characterTextureHolder;
        Widget* _buttons;
    };
}