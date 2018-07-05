#include "GrayPPBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GrayPPBar::GrayPPBar()
    {
        _slot = new ImageWidget;
        _addWidget(_slot);
        _slot->setPropSize(0.042f, 0.042f);
    }

    bool GrayPPBar::isTaken() const
    {
        return _state == State::Empty;
    }
    void GrayPPBar::take()
    {
        _state = State::Empty;
        _isTaken = true;
        _updateTextRect();
    }
    void GrayPPBar::returnTo()
    {
        _state = State::Full;
        _isTaken = false;
        _updateTextRect();
    }

    void GrayPPBar::setSlotTextures(sf::Texture* tex)
    {
        _slot->setTexture(tex);
        _state = State::Empty;
        _isTaken = true;
        _updateTextRect();
    }

    void GrayPPBar::dim()
    {
        _state = State::Dimmed;
        _updateTextRect();
    }
    void GrayPPBar::undim()
    {
        if(_isTaken) _state = State::Empty;
        else _state = State::Full;

        _updateTextRect();
    }

    void GrayPPBar::_updateTextRect()
    {
        switch(_state)
        {
            case State::Empty: _slot->setTextureRect({{0, 0}, {ppDim, ppDim}}); break;
            case State::Full: _slot->setTextureRect({{ppDim, 0}, {ppDim, ppDim}}); break;
            case State::Dimmed: _slot->setTextureRect({{2 * ppDim, 0}, {ppDim, ppDim}}); break;
        }
    }
}