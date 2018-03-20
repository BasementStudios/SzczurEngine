#pragma once
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Script/Script.hpp"
namespace rat {
    class Item {
    public:
        Item(Widget* owner);
        ~Item();

        static void initScript(Script& script);
        void setTexture(sf::Texture* texture);
        void setDescription(const std::string& text, sf::Texture* texture, sf::Font* font, size_t fontSize);
    private:
        Widget* _owner;
        ImageWidget* _sprite{nullptr};
        ImageWidget* _description{nullptr};
    };
}