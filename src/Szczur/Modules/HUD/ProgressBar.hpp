#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class ProgressBar : public sf::Drawable, public sf::Transformable {
    public:
        ProgressBar() = default;
        ProgressBar(const std::string& file, sf::Color color, sf::Color bgColor);
        void create(const std::string& file, sf::Color color, sf::Color bgColor);
        void setProgress(float value);
        void addProgress(float value);
        float getProgress() const;

    private:
        sf::Sprite _foreground;
        sf::RectangleShape _background;
        sf::RectangleShape _bar;

        float _progress;
        float _maxSize;

        sf::Texture _texture;

        void _updateSize();
        void _checkProgress();

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}