#pragma once

#include <functional>

namespace rat
{
    class GUI;
    class WindowWidget; class TextWidget; class Widget;
    class BattleButton
    {
    public:
        BattleButton();
        void setParent(Widget* parent);
        void setCallback(std::function<void(Widget*)>& call);
        void loadAssetsFromGUI(GUI& gui);
    private:
        WindowWidget* _background{nullptr};
        TextWidget* _text{nullptr};

    };
}