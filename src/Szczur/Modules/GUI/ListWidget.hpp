#pragma once

#include "Widget.hpp"

#include <vector>

namespace rat
{
    class ListWidget : public Widget
    {
    public:
        void setBetweenPadding(float padding);

        void makeVertical();
        void makeHorizontal();

        void makeFronted();
        void makeReversed();

        void popBack(size_t amount = 1);
    protected:
        virtual void _addWidget(Widget* widget) override;
        virtual void _calculateSize() override;
        virtual sf::Vector2u _getChildrenSize() override;
        virtual sf::Vector2f _getChildrenShift() const override;
        virtual sf::Vector2f _getChildShiftByIndex(size_t index) const override;
        virtual void _clear() override;

        virtual void _drawChildren(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        std::vector<sf::Vector2f> _shifts;
        bool _areShiftsCurrent{false};
        float _betweenWidgetsPadding{0.f};

        void _updateChildrenSize();
        sf::Vector2u _childrenSize;

        enum class Positioning { Horizontal, Vertical } _positioning{Positioning::Vertical};
        bool _isReversed{false};

        void _updateShifts();
    };
}