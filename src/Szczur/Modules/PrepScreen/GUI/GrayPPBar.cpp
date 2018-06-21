#include "GrayPPBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GrayPPBar::GrayPPBar()
    {
        _container = new ImageWidget;
        _addWidget(_container);
        auto dim = (unsigned int)(120 * 0.2f); 
        _container->setPropSize(0.0325f, 0.0325f);

        _pp = new ImageWidget;
        _container->add(_pp);
        _pp->setPropSize(0.03f, 0.03);
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