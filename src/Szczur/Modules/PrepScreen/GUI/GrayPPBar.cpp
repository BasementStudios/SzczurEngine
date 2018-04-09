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
        std::cout << "ppAmount: " << ppAmount << "\n";
        if(ppAmount > _activePPsAmount)
        {
            size_t realAmount = _pps.size(); 
            std::cout << "realAmount: " << realAmount << "\n";
            if(ppAmount > realAmount)
            {
                size_t amount = ppAmount - realAmount;
                _addPPWidget(amount);
            }
            _recalculatePPsFromTo(realAmount, ppAmount);
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
        std::cout << amount << "pps added\n";
        for(size_t i = 0; i < amount; i++)
        {
            std::cout << "Before adding" << std::endl;
            ImageWidget* newPP = new ImageWidget;
            newPP->setTexture(_texture);
            const auto& oldSize = static_cast<sf::Vector2f>(_texture->getSize());
            newPP->setScale({40.f/oldSize.x, 40.f/oldSize.y});
            //newPP->setScale({2.f, 2.f});
            _background->add(newPP);
            _pps.emplace_back(newPP);
            std::cout << "After adding" << std::endl;
            
        }
    }

    void GrayPPBar::_recalculatePPsFromTo(size_t from, size_t to)
    {
        for(size_t i = from; i < to; i++)
        {
            std::cout << "PPActivation index: " << i << "\n";
            _pps[i]->setPosition(float(i) * _dim, 0.f);
            _pps[i]->activate();
        }
        _activePPsAmount = to;
    }

    void GrayPPBar::_deactivatePPsFromTo(size_t from, size_t to)
    {
        for(size_t i = from; i < to; i++)
        {
            std::cout << "PPDeactivation index: " << i << "\n";            
            _pps[i]->setPosition(0.f, 0.f);
            _pps[i]->deactivate();
        }
        _activePPsAmount = from;
    }

    void GrayPPBar::initTextureViaGui(GUI& gui)
    {
        std::string path = "assets/PrepScreen/PP.png";
        gui.addAsset<sf::Texture>(path);
        _texture = gui.getAsset<sf::Texture>(path);
    }
    
    PPContainer& GrayPPBar::getSource()
    {
        return PPBar::_source;
    }
}