#pragma once

namespace rat
{
    class WindowWidget; class ImageWidget; class Widget;

    class SkillSlot
    {
    public:
        void setParent(Widget* parent);
    protected:
        WindowWidget* _background{nullptr};
        ImageWidget* _icon{nullptr};
    };
}