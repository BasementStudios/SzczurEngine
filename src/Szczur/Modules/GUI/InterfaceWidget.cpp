#include "InterfaceWidget.hpp"

namespace rat
{
    InterfaceWidget::InterfaceWidget()
    {
        makeChildrenUnresizable();
    }
    void InterfaceWidget::setWidthToHeightProp(float prop)
    {
        _widthToHeightProp = prop;
        _hasProportion = true;

        _updateSizeProportion();
        _updateSizingSize();

        forceToUpdatePropSize();
    }

    void InterfaceWidget::_addWidget(Widget* widget)
    {
        widget->setInterface(this);
    }

    void InterfaceWidget::updateSizeByWindowSize(const sf::Vector2u& winSize)
    {
        setSize(winSize);
        _updateSizeProportion();
        _updateSizingSize();

        std::cout << "Sizing: X:" << _sizingSize.x << " Y: " << _sizingSize.y << '\n';
        forceToUpdatePropSize();
    }

    sf::Vector2i InterfaceWidget::getSizeByPropSize(const sf::Vector2f& propSize) const
    {
        std::cout << "Sizing: X:" << _sizingSize.x << " Y: " << _sizingSize.y << '\n';
        return { int(_sizingSize.x * propSize.x), int(_sizingSize.y * propSize.y) };
    }

    void InterfaceWidget::_updateSizeProportion()
    {
        auto size = getMinimalSize();
        float wth = float(size.x) / float(size.y);

        if(_hasProportion)
        {
            if(wth > _widthToHeightProp)
            {
                size.x = (unsigned int)(float(size.y) * _widthToHeightProp);
            }
            else
            {
                size.y = (unsigned int)(float(size.x) / _widthToHeightProp);
            }
            setSize(size);
        }
    }

    void InterfaceWidget::_updateSizingSize()
    {
        auto innerSize = static_cast<sf::Vector2f>(getMinimalSize()) - (getPadding() * 2.f);

        _sizingSize = innerSize;

        if(!_hasSizing) return;


        if(_widthToHeightProp > _sizingWidthToHeightProp)
        {
            _sizingSize.x = innerSize.y * _widthToHeightProp;
        }
        else
        {
            _sizingSize.y = innerSize.x / _widthToHeightProp;
        }
    }
}