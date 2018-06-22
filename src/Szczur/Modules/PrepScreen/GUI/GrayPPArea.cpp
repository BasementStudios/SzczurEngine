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
    GrayPPArea::GrayPPArea()
    :
    BaseBar()
    {
        setPropOrigin(0.5f, 0.f);
        _border = new WindowWidget;
        _addWidget(_border);
        _border->setScale(0.35f, 0.35f);
        _border->setPadding(10.f, 3.f);
        //_border->setPatchAmount(1, 1);

        _ppsList = new ListWidget;
        _ppsList->makeHorizontal();
        _ppsList->setBetweenPadding(7.f);
        _ppsList->setPropPosition(0.5f, 0.5f);

        _border->add(_ppsList);
    }
    
    void GrayPPArea::setPPs(size_t activated, size_t total)
    {
        size_t totalsDiff = size_t(abs(int(total) - int(_totalAmount)));
        if(totalsDiff > 0)
        {
            if(_totalAmount < total)
            {
                for(size_t i = 0; i < totalsDiff; i++)
                {
                    _addPPBar();
                }
            }
            else
            {
                for(size_t i = 0; i < totalsDiff; i++)
                {
                    _removePPBar();
                }
            }
        }

        _totalAmount = total;

        _resetTakenPPBars();
        if(activated > total)
        {
            LOG_ERROR("");
            return;
        }
        for(size_t i = 0; i < activated; i++)
        {
            _pps[i]->returnTo();
        }
    }

    void GrayPPArea::_addPPBar()
    {
        auto ppBar = std::make_unique<GrayPPBar>();
        ppBar->setContainerTexture(_containerTex);
        ppBar->setPPTexture(_ppTex);
        ppBar->setParent(_ppsList);
        _pps.emplace_back(std::move(ppBar));
    }
    void GrayPPArea::_removePPBar()
    { 
        if(_totalAmount == 0) return;
        
        _pps.pop_back();
        _ppsList->popBack();
    }   

    void GrayPPArea::_resetTakenPPBars()
    {   
        for(auto& ppBar : _pps)
        {
            ppBar->take();
        }
    }

    void GrayPPArea::dimPPs(size_t amount)
    {
        
    }
    void GrayPPArea::undimPPs()
    {
        
    }

    void GrayPPArea::initAssetsViaGUI(GUI& gui)
    {
        _ppTex = gui.getAsset<sf::Texture>("Assets/PrepScreen/GrayPP.png");
        _containerTex = gui.getAsset<sf::Texture>("Assets/Test/GlyphCircle.png");
        auto* borderTex = gui.getAsset<sf::Texture>("Assets/Test/GrayPPWindow.png");
        _border->setTexture(borderTex, 144, 76);
        _border->setPadding(0.f, 0.f);
    }
}