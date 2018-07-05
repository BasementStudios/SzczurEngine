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
        _border = new WindowWidget;
        _addWidget(_border);
        _border->setMainPatchPropSize({0.326f, 0.0772f});
        _border->setPropSize(0.768f, 0.0772f);
        _border->setPropPadding(0.035f, 0.f);

        _ppsList = new ListWidget;
        _ppsList->makeHorizontal();
        _ppsList->setBetweenPadding(7.f);
        _ppsList->setPropPosition(0.5f, 0.5f);

        _border->add(_ppsList);

        _getBase()->setPropPosition(0.5f, 0.f);
    }
    
    void GrayPPArea::setPPs(const size_t activated)
    {
        if(activated == _activated) return;

        if(activated > _pps.size())
        {
            LOG_ERROR(activated, " PPs cannot be activated, not enough slots...");
            return;
        }

        for(size_t i = 0; i < activated; ++i) _pps[i]->returnTo();
        for(size_t i = activated; i < _pps.size(); ++i) _pps[i]->take();

        _activated = activated;
    }

    void GrayPPArea::setMaxPPs(size_t maxPPs)
    {
        size_t totalsDiff = size_t(abs(int(maxPPs) - int(_pps.size())));
        if(totalsDiff > 0)
        {
            if(_slots < maxPPs)
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
            _slots = maxPPs;
        }
    }

    void GrayPPArea::_addPPBar()
    {
        auto ppBar = std::make_unique<GrayPPBar>();
        ppBar->setSlotTextures(_slotTex);
        ppBar->setParent(_ppsList);
        _pps.emplace_back(std::move(ppBar));
    }
    void GrayPPArea::_removePPBar()
    { 
        if(_pps.size() == 0) return;
        
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
        size_t ppsAmount = _pps.size();
        if(amount > ppsAmount)
        {
            LOG_ERROR("GrayPPArea::dimPPs tried to dim ", amount, " not existing pps");
            return;
        }
        for(auto& pp : _pps) pp->undim();
        for(size_t i = ppsAmount - amount; i < ppsAmount; ++i)
        {
            auto& pp = _pps[i];
            pp->dim();
        }

    }
    void GrayPPArea::undimPPs()
    {
        for(auto& pp : _pps)
        {
            pp->undim();
        }
    }

    void GrayPPArea::initAssetsViaGUI(GUI& gui)
    {
        _slotTex = gui.getTexture("Assets/PrepScreen/PPSlots.png");

        auto* borderTex = gui.getAsset<sf::Texture>("Assets/Test/GrayPPWindow.png");
        _border->setTexture(borderTex, 144, 76);
    }
}