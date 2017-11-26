#include "CheckWidget.hpp"

namespace rat {
    CheckWidget::CheckWidget(const std::string& path1, const std::string& path2) :
    Widget(),
    _isTriggered(false)
    {
        on = new ImageWidget(path1);
        off = new ImageWidget(path2);

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