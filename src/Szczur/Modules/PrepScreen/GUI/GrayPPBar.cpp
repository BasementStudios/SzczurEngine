#include "GrayPPBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GrayPPBar::GrayPPBar()
    {
        _container = new ImageWidget;
        _addWidget(_container);
        _container->setPropSize(0.0425f, 0.0425f);

        _pp = new ImageWidget;
        _container->add(_pp);
        _pp->setPropSize(0.035f, 0.035);
        _pp->setPropPosition(0.5f, 0.5f);
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