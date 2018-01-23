#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"
#include "CheckWidget.hpp"

namespace rat {
    class RadioWidget : public Widget {
    public:
        RadioWidget(size_t amount, sf::Texture* text1, sf::Texture* text2);

        size_t whichTriggered() const;

    private:
        size_t _whichTriggered;
    };
}