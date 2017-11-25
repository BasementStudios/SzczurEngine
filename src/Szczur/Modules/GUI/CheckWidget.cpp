#include "CheckWidget.hpp"

namespace rat {
    CheckWidget::CheckWidget(const std::string& path1, const std::string& path2) :
    Widget(),
    _isTriggered(false)
    {
        ImageWidget* on = new ImageWidget(path1);
        ImageWidget* off = new ImageWidget(path2);

        add(on);
        add(off);

        on->invisible();

        setCallback(CallbackType::onRelease, [on, off, this](Widget*){
            if(_isTriggered) {
                on->invisible();
                off->visible();
                _isTriggered = false;
            }
            else {
                on->visible();
                off->invisible();
                _isTriggered = true;
            }
        });
    }

    bool CheckWidget::isTriggered() const {
        return _isTriggered;
    }
}