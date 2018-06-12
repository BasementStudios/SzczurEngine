#pragma once

#include "Widget.hpp"

namespace rat
{
    class InterfaceWidget : public Widget
    {
    public:
        void updateSizeByWindowSize(const sf::Vector2u& winSize);
        sf::Vector2i getSizeByPropSize(const sf::Vector2f& propSize);

        void setWidthToHeightProp(float prop);
    protected:
        virtual void _update(float deltaTime) override;
        virtual void _addWidget(Widget* widget) override;
    private:
        bool _childrenSizesMustBeenRecalculated{false};
        bool _hasProportion{false};
        float _widthToHeightProp{16.f/9.f};

        bool _hasSizing{false};
        float _sizingWidthToHeightProp{16.f/9.f};
        sf::Vector2f _sizingSize;
        enum class SizeState : char {TooWidth, TooHeigh, Ok} _sizeState{SizeState::Ok};

        void _updateSizeProportion();
        void _updateSizingSize();
    };
}
