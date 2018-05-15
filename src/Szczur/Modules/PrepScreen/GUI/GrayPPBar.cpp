#include "GrayPPBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GrayPPBar::GrayPPBar()
    {
        _container = new ImageWidget;
        _addWidget(_container);
        _container->setSize(50, 50);
        float padding = _border * 50.f / 1150.f;
        _container->setPadding(padding, padding);

        _pp = new ImageWidget;
        _container->add(_pp);
        unsigned int ppDim = 50 - (unsigned int)(2.f * padding);
        _pp->setSize(ppDim, ppDim);
    }

    bool GrayPPBar::isTaken() const
    {
        return _pp->isVisible();
    }
    void GrayPPBar::take()
    {
        _pp->invisible();
    }
    void GrayPPBar::returnTo()
    {
        _pp->visible();
    }

    void GrayPPBar::setContainerTexture(sf::Texture* tex)
    {
        _container->setTexture(tex);
    }
    void GrayPPBar::setPPTexture(sf::Texture* tex)
    {
        _pp->setTexture(tex);
    }
}