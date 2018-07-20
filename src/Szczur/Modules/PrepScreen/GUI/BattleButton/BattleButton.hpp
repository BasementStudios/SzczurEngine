#pragma once

#include <functional>

#include <sol.hpp>

namespace rat
{
    class GUI;
    class WindowWidget; class TextWidget; class Widget;
    class BattleButton
    {
    public:
        BattleButton();
        void setParent(Widget* parent);
        void setCallback(sol::function func);
        void setCallback(const std::function<void(Widget*)>& func);
        void loadAssetsFromGUI(GUI& gui);
    private:
        WindowWidget* _background{nullptr};
        TextWidget* _text{nullptr};

    };
}