#include "GrayPPBar.hpp"

#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GrayPPBar::GrayPPBar()
    {
        _slot = new ImageWidget;
        _addWidget(_slot);
        _slot->setPropSize(0.05f, 0.05f);
    }

    bool GrayPPBar::isEmpty() const
    {
        return _state == State::Empty;
    }

    void GrayPPBar::fill()
    {
        assert(_state != State::Locked);
        _state = State::Full;
        _updateTextRect();
    }
    void GrayPPBar::empty()
    {
        assert(_state != State::Locked);
        _state = State::Empty;
        _updateTextRect();
    }
    void GrayPPBar::lock()
    {
        _state = State::Locked;
        _updateTextRect();
    }
    void GrayPPBar::unlock()
    {
        if(_state != State::Locked) return;
        _state = State::Empty;
        _updateTextRect();
    }

    void GrayPPBar::setSlotTextures(sf::Texture* tex)
    {
        _slot->setTexture(tex);
        _state = State::Locked;
        _updateTextRect();
    }

    void GrayPPBar::dim()
    {
        _isDimmed = true;
        _updateTextRect();
    }
    void GrayPPBar::undim()
    {
        _isDimmed = false;
        _updateTextRect();
    }

    void GrayPPBar::_updateTextRect()
    {
        if(_state == State::Locked)
        {
            _slot->setTextureRect({{3 * ppDim, 0}, {ppDim, ppDim}});
            _isDimmed = false;
        }
        else if(_isDimmed)
        {
            _slot->setTextureRect({{2 * ppDim, 0}, {ppDim, ppDim}});
        }
        else
        {
            if(_state == State::Empty) 
            {
                _slot->setTextureRect({{0, 0}, {ppDim, ppDim}});
            }
            else 
            {
                _slot->setTextureRect({{ppDim, 0}, {ppDim, ppDim}});
            }
        }
    }
}