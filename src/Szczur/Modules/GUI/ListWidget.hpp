#pragma once

#include "Widget.hpp"

#include <vector>

namespace rat
{
    class Script;

    class ListWidget : public Widget
    {
    public:
        void setBetweenPadding(float padding);
        void setPropBetweenPad(float propPad);
        void setAutoBetweenPadding();

        void makeVertical();
        void makeHorizontal();

        void makeFronted();
        void makeReversed();

        void popBack(size_t amount = 1);
        
    public:
        static void initScript(Script& script);

    protected:
        virtual void _calculateSize() override;
        virtual sf::Vector2f _getChildrenSize() override;
        virtual void _clear() override;
        virtual sf::Vector2f _getInnerSize() const override;
        virtual void _recalcElementsPropSize() override;
        virtual void _recalcChildrenPos() override;
    private:
        float _betweenWidgetsPadding{0};
        float _propBetweenPad{1.f};

        bool _hasPropBetweenPad{false};
        bool _hasAutoBetweenPad{false};

        void _calcPropBetweenPad();

        void _updateChildrenSize();
        sf::Vector2f _childrenSize;

        void _calculateAutoBetweenPad();

        enum class Positioning { Horizontal, Vertical } _positioning{Positioning::Vertical};
        bool _isReversed{false};
    };
}