#include "ProgressBar.hpp"

namespace rat {
    ProgressBar::ProgressBar(const std::string& file, sf::Color color, sf::Color bgColor) :
    _progress(1.f) {
        create(file, color, bgColor);
    }
    
    void ProgressBar::create(const std::string& file, sf::Color color, sf::Color bgColor) {
        _progress = 1.f;
        _texture.loadFromFile(file);
        _foreground.setTexture(_texture);

        _background.setSize(static_cast<sf::Vector2f>(_texture.getSize()));
        _background.setFillColor(bgColor);

        _bar.setSize(static_cast<sf::Vector2f>(_texture.getSize()));
        _bar.setFillColor(color);

        _maxSize = _bar.getSize().x;
    }

    void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(_background, states);
        target.draw(_bar, states);
        target.draw(_foreground, states);
    }

    void ProgressBar::setProgress(float value) {
        _progress = value;
        _checkProgress();
        _updateSize();
    }

    void ProgressBar::addProgress(float value) {
        _progress += value;
        _checkProgress();
        _updateSize();
    }

    float ProgressBar::getProgress() const {
        return _progress;
    }

    void ProgressBar::_updateSize() {
        _bar.setSize({_progress*_maxSize, _bar.getSize().y});
    }

    void ProgressBar::_checkProgress() {
        if(_progress > 1.f)
            _progress = 1.f;
        else if(_progress < 0.f)
            _progress = 0.f;
    }
}