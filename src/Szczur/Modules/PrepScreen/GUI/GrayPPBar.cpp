#include "GrayPPBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GrayPPBar::GrayPPBar()
    {
        _container = new ImageWidget;
        _addWidget(_container);
        _container->setPropSize(0.06f, 0.06f);

        _pp = new ImageWidget;
        _container->add(_pp);
        _pp->setPropSize(0.05f, 0.05f);
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

    void GrayPPBar::dim()
    {
        _getBase()->setColorInTime({150, 150, 150}, 0.3f);
    }
    void GrayPPBar::undim()
    {
        _getBase()->setColorInTime({255, 255, 255}, 0.3f);
    }
}