#include "RadioWidget.hpp"

#include <iostream>

namespace rat {
    RadioWidget::RadioWidget(size_t amount, const std::string& path1, const std::string& path2) :
    Widget(),
    _whichTriggered(0u) {

        std::vector<CheckWidget*> checks;
        for(size_t i = 0; i<amount; ++i)
            checks.push_back(new CheckWidget(path1, path2));

        size_t i = 0;
        for(CheckWidget* check : checks) {
            check->setCallback(CallbackType::onRelease, [this, checks, check, i](Widget*){
                for(CheckWidget* it : checks) {
                    it->disable();
                }
                check->enable();
                _whichTriggered = i;
            });
            i++;
            check->move({0.f, 50.f * i});
            add(check);
        }


    }

    size_t RadioWidget::whichTriggered() const {
        return _whichTriggered;
    }
}