#pragma once

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Script/Script.hpp"


#include "TextManager.hpp"

#include "Options.hpp"

namespace rat {
    class TextAreaWidget;
    class TextWidget;
    class ImageWidget;
    class Widget;
    class ScrollAreaWidget;
    class DialogGUI {
    public:
        DialogGUI(GUI& gui);

        ~DialogGUI();

        void hide();

        void show();

        static void initScript(Script& script);

        void clear();

        void setText(const std::string& text);

        void interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t, size_t, bool)> callback);

        void setInterface(Widget* _interface);
        Widget* getInterface() const;

        void setButtonsContainer(Widget* container);
        const sol::function& getButtonsContainer() const;

        void setButtonsCreator(const sol::function& func);
        Widget* getButtonsCreator() const;

        void setArea(TextAreaWidget* area);
        TextAreaWidget* getArea() const;

    private:
        Widget* _interface;
        sol::function _buttonsCreator;
        Widget* _buttonsContainer;
        TextAreaWidget* _area;
    };
}