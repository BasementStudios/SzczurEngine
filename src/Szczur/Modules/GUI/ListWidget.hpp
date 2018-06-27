#pragma once

#include "Widget.hpp"

#include <vector>

namespace rat
{
    class ListWidget : public Widget
    {
    public:
        void setBetweenPadding(int padding);
        void setAutoBetweenPadding();

        void makeVertical();
        void makeHorizontal();

        void makeFronted();
        void makeReversed();

        void popBack(size_t amount = 1);

    protected:
        virtual void _calculateSize() override;
        virtual sf::Vector2i _getChildrenSize() override;

        virtual void _recalcChildrenPos() override;
    private:
        int _betweenWidgetsPadding{0};
        bool _hasAutoBetweenPad{false};

        void _updateChildrenSize();
        sf::Vector2i _childrenSize;

        void _calculateAutoBetweenPad();

        enum class Positioning { Horizontal, Vertical } _positioning{Positioning::Vertical};
        bool _isReversed{false};
    };
}