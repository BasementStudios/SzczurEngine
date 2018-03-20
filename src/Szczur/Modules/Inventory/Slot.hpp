#pragma once
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Item.hpp"

namespace rat {
    class Slot {
    public:
        Slot() {

        }

        ~Slot() {
            if(_holder)
                delete _holder;
        }

        void setItem(Item* item) {
            _holder = item;
        }

        Item* getItem() const {
            return _holder;
        }

        void setWidget(ImageWidget* widget) {
            _widget = widget;
        }
        ImageWidget* getWidget() const {
            return _widget;
        }
        bool isEmpty() const {
            return !static_cast<bool>(_holder);
        }
    private:
        ImageWidget* _widget{nullptr};
        Item* _holder{nullptr};
    };
}