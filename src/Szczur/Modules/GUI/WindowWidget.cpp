#include "WindowWidget.hpp"

#include <cassert>

#include "Szczur/Utility/Logger.hpp"


#include "InterfaceWidget.hpp"
#include "Widget-Scripts.hpp"

namespace rat
{
    WindowWidget::WindowWidget()
    :
    Widget()
    {
        setScale({0.2f, 0.2f});
    }

    void WindowWidget::initScript(Script& script) 
    {
        auto object = script.newClass<WindowWidget>("WindowWidget", "GUI");

        gui::WidgetScripts::set(object);

        object.set("setMainPatchPropSize", &WindowWidget::setMainPatchPropSize);

        object.setOverload("setTexture",
            static_cast<void (WindowWidget::*)(sf::Texture*, int)>(&WindowWidget::setTexture),
            static_cast<void (WindowWidget::*)(sf::Texture*, int, int)>(&WindowWidget::setTexture)
        );
        object.setOverload("setScale",
            static_cast<void (WindowWidget::*)(const sf::Vector2f&)>(&WindowWidget::setScale),
            static_cast<void (WindowWidget::*)(float, float)>(&WindowWidget::setScale)
        );

        object.init();
    }


    void WindowWidget::setTexture(sf::Texture* texture, int paddingWidth, int paddingHeight)
    {
        _ninePatch.setTexture(texture, paddingWidth, paddingHeight);
        if(_isMainPatchPropSizeSet && _interface) _calcMainPatchSize();
    }
    
    void WindowWidget::setTexture(sf::Texture* texture, int padding)
    {
        setTexture(texture, padding, padding);
    }
    void WindowWidget::setScale(const sf::Vector2f& scale)
    {
        _ninePatch.setScale(scale);
        _scale = scale;
    }
    void WindowWidget::setScale(float x, float y)
    {
        setScale({x, y});
    }

    void WindowWidget::setMainPatchPropSize(const sf::Vector2f& propSize)
    {
        _isMainPatchPropSizeSet = true;
        _mainPathPropSize = propSize;
        if(_interface) _calcMainPatchSize();
        else _elementsPropSizeMustBeenCalculated = true;
    }
    void WindowWidget::_calcMainPatchSize()
    {
        assert(_isMainPatchPropSizeSet);
        assert(_interface);
        if(auto* tex = _ninePatch.getTexture(); tex != nullptr)
        {
            auto texSize = static_cast<sf::Vector2f>(tex->getSize());
            auto size = _interface->getSizeByPropSize(_mainPathPropSize);

            sf::Vector2f scale = {size.x / texSize.x, size.y / texSize.y};
            setScale(scale);
        }
    }
    void WindowWidget::_recalcElementsPropSize()
    {
        if(_isMainPatchPropSizeSet) _calcMainPatchSize();
    }
    


    void WindowWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_ninePatch, states);
    }
    void WindowWidget::_setColor(const sf::Color& color)
    {
        _ninePatch.setColor(color);
    }
    
	sf::Vector2f WindowWidget::_getSize() const
    {
        return _minWinSize;
    }
    
    void WindowWidget::setPatchAmount(const sf::Vector2u& amount)
    {
        _isPathesAmountSet = true;
        _patchesAmount = amount;
        _calcPatchesAmount();
        _aboutToRecalculate = true;
    }
    void WindowWidget::setPatchAmount(unsigned int horizontalAmount, unsigned int verticalAmount)
    {
        setPatchAmount({horizontalAmount, verticalAmount});
    }
	void WindowWidget::_calculateSize()
    {
        _calcPatchesAmount();
        auto size = getSize();
        size.x = std::max(_minWinSize.x, size.x);
        size.y = std::max(_minWinSize.y, size.y);
        _ninePatch.setSize(size);
    }
    void WindowWidget::_calcPatchesAmount()
    {
        if(_isPathesAmountSet)
        {
            auto innerElSize = _ninePatch.getInnerPathSize();
            sf::Vector2f innerSize = {innerElSize.x * float(_patchesAmount.x), innerElSize.y * float(_patchesAmount.y)};
            auto newSize = innerSize + _ninePatch.getCornersCombinedSize();
            _minWinSize = newSize;
        }
    }

    void WindowWidget::_recalcPos()
    {
        _ninePatch.setPosition(sf::Vector2f(gui::FamilyTransform::getDrawPosition()));
    }


}