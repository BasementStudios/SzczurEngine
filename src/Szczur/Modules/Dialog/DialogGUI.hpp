#pragma once

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "TextManager.hpp"

#include "Options.hpp"

namespace rat {
    class DialogGUI {
    public:
        DialogGUI(GUI& gui);

        ~DialogGUI();

        static void initScript(Script& script);

        void clear();

        void setText(const std::string& text);

        void setCharacterName(const std::string& name);

        void setCharacterTexture(sf::Texture* texture);

        void interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t)> callback);

        void setButtonsContainer(Widget* container);
        const sol::function& getButtonsContainer() const;

        void setButtonsCreator(const sol::function& func);
        Widget* getButtonsCreator() const;

        void setArea(TextAreaWidget* area);
        TextAreaWidget* getArea() const;

        void setCharacter(ImageWidget* image);
        ImageWidget* getCharacter() const;

        void setName(TextWidget* name);
        TextWidget* getName() const;
    private:
        sol::function _buttonsCreator;
        Widget* _buttonsContainer;
        TextAreaWidget* _area;
        ImageWidget* _character;
        TextWidget* _name;
    };
}