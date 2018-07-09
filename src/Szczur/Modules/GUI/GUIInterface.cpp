#include "GUIInterface.hpp"

namespace rat
{
    void GUIInterface::updateSizeByWindowSize(const sf::Vector2u& winSize)
    {
        auto winSizeFloat = static_cast<sf::Vector2f>(winSize); 
        if(_hasProportion)
        {
            float winProp = winSizeFloat.x / winSizeFloat.y;
            if(winProp > _widthToHeightProp)
            {
                _size.y = winSizeFloat.y;
                _size.x = _widthToHeightProp * _size.y;
            }
            else
            {
                _size.x = winSizeFloat.x;
                _size.y = _size.x / _widthToHeightProp;
            }
        }
        else
        {
            _size = winSizeFloat;
        }
    }
    /*
    const sf::Vector2f& GUIInterface::getSize() const;
        void GUIInterface::setWidthToHeightProportion(float prop);*/
} 