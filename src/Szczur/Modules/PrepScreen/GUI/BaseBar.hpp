#pragma once

#include <memory>

namespace rat
{
    class Widget; class ImageWidget;

    class BaseBar
    {
    public:
        BaseBar();
        void setPosition(float x, float y);
        void setParent(Widget* parent);

    protected:
        void _addWidget(Widget* addon);
        void _setWidgetSize(ImageWidget* widget, float x, float y);
        template<typename B>
        void _addBar(std::unique_ptr<B>& bar);
    private:
        Widget* _base{nullptr};

    };

    template<typename B>
    void BaseBar::_addBar(std::unique_ptr<B>& bar)
    {
        bar->setParent(_base);
    }
}