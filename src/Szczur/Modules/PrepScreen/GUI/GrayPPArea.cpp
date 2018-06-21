#include "GrayPPArea.hpp"

#include <memory>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "../ResourcesContainer.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GrayPPArea::GrayPPArea(ResourcesContainer& source)
    :
    _source(source),
    _activePPsAmount{0},
    BaseBar()
    {
        setPropOrigin(0.5f, 0.f);
        _border = new WindowWidget;
        _addWidget(_border);
        _border->setScale(0.2f, 0.2f);
        _border->setPatchAmount(1, 1);

        _ppsList = new ListWidget;
        _ppsList->makeHorizontal();
        _ppsList->setBetweenPadding(7.f);
        _ppsList->setPropPosition(0.5f, 0.5f);

        _border->add(_ppsList);
    }


    ResourcesContainer& GrayPPArea::getSource()
    {
        return _source;
    }

    void GrayPPArea::recalculate()
    {
        const auto& source = _source.ppContainer;
        size_t sourceTotalAmount = source.getTotalAmount();

        auto diff = (size_t)(abs(int(_activePPsAmount) - int(sourceTotalAmount)));

        if(_activePPsAmount < sourceTotalAmount)
        {
            for(size_t i = 0; i < diff; i++)
            {
                _addPPBar(); 
            }
        }
        else if(_activePPsAmount > sourceTotalAmount)
        {
            for(size_t i = 0; i < diff; i++)
            { 
                _removePPBar();
            }
        }
        
        _resetTakenPPBars();
        _takeNecessaryPPBars();
    }   

    void GrayPPArea::_addPPBar()
    {
        
        auto index = _activePPsAmount++;

        if(_pps.size() <= index)
        {
            auto ppBar = std::make_unique<GrayPPBar>();
            ppBar->setContainerTexture(_containerTex);
            ppBar->setPPTexture(_ppTex);
            ppBar->setParent(_ppsList);
            _pps.emplace_back(std::move(ppBar));
        }

        auto& ppBar = _pps[index];
        ppBar->activate();
        
    }
    void GrayPPArea::_removePPBar()
    {
        
        if(_activePPsAmount == 0) return;
        auto index = --_activePPsAmount;

        auto& ppBar = _pps[index];
        ppBar->setPosition(0, 0);
        ppBar->deactivate();
        
    }   

    void GrayPPArea::_resetTakenPPBars()
    {   
        for(auto& ppBar : _pps)
        {
            ppBar->returnTo();
        }
    }
    void GrayPPArea::_takeNecessaryPPBars()
    {
        
        auto& source = _source.ppContainer;
        auto takenAmount = source.getTotalAmount() - source.getAmount();
        for(size_t i = 0; i < takenAmount; i++)
        {
            auto& ppBar = _pps[i];
            ppBar->take();
        }
        
    }
    

    void GrayPPArea::initAssetsViaGUI(GUI& gui)
    {
        _ppTex = gui.getAsset<sf::Texture>("Assets/PrepScreen/GrayPP.png");
        _containerTex = gui.getAsset<sf::Texture>("Assets/PrepScreen/GrayPP.png");
        auto* borderTex = gui.getAsset<sf::Texture>("Assets/Test/GrayPPWindow.png");
        _border->setTexture(borderTex, 144, 76);
        _border->setPadding(0.f, 0.f);
    }
}