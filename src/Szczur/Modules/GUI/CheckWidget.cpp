#include "CheckWidget.hpp"

namespace rat {
    CheckWidget::CheckWidget(sf::Texture* text1, sf::Texture* text2) :
    Widget(),
    _isTriggered(false)
    {
        on = new ImageWidget(text1);
        off = new ImageWidget(text2);

        add(on);
        add(off);

        on->invisible();

        setCallback(CallbackType::onRelease, [this](Widget*){
            if(_isTriggered)
                disable();
            else
                enable();
        });
    }

    void CheckWidget::enable() {
        on->visible();
        off->invisible();
        _isTriggered = true;
    }

    void CheckWidget::disable() {
        on->invisible();
        off->visible();
        _isTriggered = false;
    }

    bool CheckWidget::isTriggered() const {
        return _isTriggered;
    }
}