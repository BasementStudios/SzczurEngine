#pragma once

#include "Widget.hpp"

namespace rat
{
    class InterfaceWidget : public Widget
    {
    public:
        InterfaceWidget();
        void updateSizeByWindowSize(const sf::Vector2u& winSize);
        sf::Vector2i getSizeByPropSize(const sf::Vector2f& propSize) const;

        void setWidthToHeightProp(float prop);
        void setSizingWidthToHeightProportion(float prop);
    protected:
        virtual void _addWidget(Widget* widget) override;
    private:
        bool _hasProportion{false};
        float _widthToHeightProp{16.f/9.f};

        bool _hasSizing{false};
        float _sizingWidthToHeightProp{16.f/9.f};
        sf::Vector2f _sizingSize;

        void _updateSizeProportion();
        void _updateSizingSize();
    };
}
