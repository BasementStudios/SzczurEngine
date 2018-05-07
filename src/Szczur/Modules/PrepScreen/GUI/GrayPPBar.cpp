#include "GrayPPBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GrayPPBar::GrayPPBar(PPContainer& source)
    :
    PPBar(source)
    {
    }

    void GrayPPBar::recalculate()
    {
        size_t ppAmount = _source.getPPAmount(); 
        if(ppAmount > _activePPsAmount)
        {
            size_t realAmount = _pps.size(); 
            if(ppAmount > realAmount)
            {
                size_t amount = ppAmount - realAmount;
                _addPPWidget(amount);
            }
            _recalculatePPsFromTo(_activePPsAmount, ppAmount);
        }
        else if(ppAmount < _activePPsAmount)
        {          
            _deactivatePPsFromTo(ppAmount, _activePPsAmount);
        }
        recalcOrigin();
    }

    void GrayPPBar::_resetPPWidget(ImageWidget* pp)
    {
        pp->setPosition(0.f, 0.f);
        pp->deactivate();
    }

    void GrayPPBar::_addPPWidget(size_t amount)
    {
        for(size_t i = 0; i < amount; i++)
        {
            ImageWidget* newPP = new ImageWidget;
            newPP->setTexture(_texture);
            const auto& oldSize = static_cast<sf::Vector2f>(_texture->getSize());
            newPP->setScale({40.f/oldSize.x, 40.f/oldSize.y});
            //newPP->setScale({2.f, 2.f});
            _background->add(newPP);
            _pps.emplace_back(newPP);            
        }
    }

    void GrayPPBar::_recalculatePPsFromTo(size_t from, size_t to)
    {
        for(size_t i = from; i < to; i++)
        {
            _pps[i]->setPosition(float(i) * _dim, 0.f);
            _pps[i]->activate();
        }
        _activePPsAmount = to;
    }

    void GrayPPBar::_deactivatePPsFromTo(size_t from, size_t to)
    {
        for(size_t i = from; i < to; i++)
        {
            _pps[i]->setPosition(0.f, 0.f);
            _pps[i]->deactivate();
        }
        _activePPsAmount = from;
    }

    void GrayPPBar::initTextureViaGui(GUI& gui)
    {
        std::string path = "Assets/PrepScreen/PP.png";
        gui.addAsset<sf::Texture>(path);
        _texture = gui.getAsset<sf::Texture>(path);
    }
    
    PPContainer& GrayPPBar::getSource()
    {
        return PPBar::_source;
    }
}