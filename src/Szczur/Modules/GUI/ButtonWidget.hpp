#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <variant>

#include "Widget.hpp"

namespace rat {
    class ButtonWidget : public Widget {
    public:
        ButtonWidget(Widget* parent, std::function<bool(Widget*)> callback = NULL);
        ButtonWidget(Widget* parent, std::function<bool()> callback = NULL);

        ButtonWidget* setCallback(std::function<bool(Widget*)> callback);
        ButtonWidget* setCallback(std::function<bool()> callback);
    private:
        bool _isHovered;

        bool _callbackWithWidget;

        std::variant<
            std::function<bool(Widget*)>,
            std::function<bool()>
        > _callback;

        bool _input(const sf::Event& event) override;
		void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void _update(float deltaTime) override;
    };
}

