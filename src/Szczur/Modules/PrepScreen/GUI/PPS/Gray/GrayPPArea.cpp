#include "GrayPPArea.hpp"

#include <memory>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "Szczur/Modules/PrepScreen/ResourcesContainer.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GrayPPArea::GrayPPArea()
    :
    BaseBar()
    {
        _ppsList = new ListWidget;
        _addWidget(_ppsList);
        _ppsList->makeHorizontal();
        _ppsList->setBetweenPadding(7.f);
        _ppsList->setPropPosition(0.5f, 0.5f);

        _getBase()->setPropPosition(0.5f, 0.f);
    }

    void GrayPPArea::setMaximum(int amount)
    {
        int maximum = int(_pps.size());
        int totalsDiff = abs(amount - maximum);
        if(totalsDiff > 0)
        {
            if(maximum < amount)
            {
                for(int i = 0; i < totalsDiff; i++)
                {
                    _addPPBar();
                }
            }
            else
            {
                for(int i = 0; i < totalsDiff; i++)
                {
                    _removePPBar();
                }
            }
        }
    }

    void GrayPPArea::_addPPBar()
    {
        auto ppBar = std::make_unique<GrayPPBar>();
        ppBar->setSlotTextures(_slotTex);
        ppBar->setParent(_ppsList);
        ppBar->lock();
        _pps.emplace_back(std::move(ppBar));
    }
    void GrayPPArea::_removePPBar()
    { 
        if(_pps.size() == 0) return;
        
        _pps.pop_back();
        _ppsList->popBack();
    } 

    void GrayPPArea::setUnlocked(int amount)
    {
        assert(amount <= _pps.size());
        for(int i = 0; i < amount; ++i)
        {
            _pps[i]->unlock();
        }
        for(int i = amount; i < _pps.size(); ++i)
        {
            _pps[i]->lock();
        }
        _slots = amount;
    }
    void GrayPPArea::setFilled(int amount)
    {
        assert(amount <= _slots);

        for(int i = 0; i < amount; ++i)
        {
            _pps[i]->fill();
        }
        for(int i = amount; i < _slots; ++i)
        {
            _pps[i]->empty();
        }

        _filled = amount;
    }

    void GrayPPArea::dimFilledPPs(int amount)
    {
        if(amount > _filled)
        {
            LOG_ERROR("GrayPPArea::dimPPs tried to dim ", amount, " not existing pps");
            return;
        }
        undimPPs();

        for(int i = _filled - amount; i < _filled; ++i)
        {
            auto& pp = _pps[i];
            pp->dim();
        }
    }
    void GrayPPArea::dimEmptyPPs(int amount)
    {
        if(amount > (_slots - _filled))
        {
            LOG_ERROR("GrayPPArea::dimPPs tried to dim ", amount, " not existing pps");
            return;
        }

        undimPPs();
        for(int i = _filled; i < _filled + amount; ++i)
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
    }
}