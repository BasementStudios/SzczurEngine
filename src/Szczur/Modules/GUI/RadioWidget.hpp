#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"
#include "CheckWidget.hpp"

namespace rat {
    class RadioWidget : public Widget {
    public:
        RadioWidget(size_t amount, const std::string& path1, const std::string& path2);

        size_t whichTriggered() const;

    private:
        size_t _whichTriggered;
    };
}