#include "Equipment.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    Equipment::Equipment()
    {
        _border = new WindowWidget;
        _addWidget(_border);
        _border->setScale(0.2f, 0.2f);
    }

    void Equipment::initAssetsViaGUI(GUI& gui)
    {
        _border->setTexture(gui.getAsset<sf::Texture>("Asstes/Test/Window.hpp"), 200);
    }
} // rat
