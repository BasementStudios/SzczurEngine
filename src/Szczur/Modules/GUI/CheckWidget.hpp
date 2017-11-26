#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "Widget.hpp"
#include "ImageWidget.hpp"

namespace rat {
    class CheckWidget : public Widget {
    public:
        CheckWidget(const std::string& path1, const std::string& path2);
        
        bool isTriggered() const;
        void enable();
        void disable();
    private:
        bool _isTriggered;

        ImageWidget* on;
        ImageWidget* off;
    };
}