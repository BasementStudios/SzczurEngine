#pragma once

#include "Szczur/Modules/GUI/GUI.hpp"

#include "Options.hpp"

namespace rat {
    class DialogGUI {
    public:
        DialogGUI(GUI& gui);

        ~DialogGUI();

        void create();

        void clear();

        void interpretOptions(Options& options, std::function<void(size_t)> callback);

        void setText(const std::string& text);
    private:
        GUI& _gui;
        Interface* _interface;
        TextAreaWidget* _area;
        Widget* _buttons;
    };
}