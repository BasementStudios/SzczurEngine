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

        void setCharacterName(const std::string& name);

        void setCharacterTexture(sf::Texture* texture);

        void interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t)> callback);

        void setKillerCallback(const std::function<void()>& killerCallback);
        const std::function<void()>& getKillerCallback() const;

        void setInterface(Widget* interface);
        Widget* getInterface() const;

        void setButtonsContainer(Widget* container);
        const sol::function& getButtonsContainer() const;

        void setButtonsCreator(const sol::function& func);
        Widget* getButtonsCreator() const;

        void setArea(TextAreaWidget* area);
        TextAreaWidget* getArea() const;

        void setCharacterHolder(Widget* holder);
        Widget* getCharacterHolder() const;

        void setName(TextWidget* name);
        TextWidget* getName() const;
    private:
        Widget* _interface;
        sol::function _buttonsCreator;
        Widget* _buttonsContainer;
        TextAreaWidget* _area;
        Widget* _characterHolder;
        ImageWidget* _character;
        TextWidget* _name;
        std::function<void()> _killerCallback;
    };
}