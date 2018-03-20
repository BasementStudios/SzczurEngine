#pragma once

#include <SFML/Graphics.hpp>
#include "Widget.hpp"


namespace rat {
    class ImageWidget;
    class CheckWidget : public Widget {
    public:
        CheckWidget(sf::Texture* text1, sf::Texture* text2);
        
        bool isTriggered() const;
        void enable();
        void disable();
    private:
        bool _isTriggered;

        ImageWidget* on;
        ImageWidget* off;
    };
}