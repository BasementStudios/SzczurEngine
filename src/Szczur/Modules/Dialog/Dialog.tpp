
#include "Szczur/Debug/Logger.hpp"
namespace rat {
    template<typename Tuple>
    Dialog::Dialog(Tuple&& tuple, const std::string& file) :
    Module(tuple) {
        /*auto& gui = getModule<GUI>();
        gui.addAsset<Json>(file);
        gui.addAsset<sf::Texture>("data/dial_back.png");

        _interface = gui.addInterface(file);*/
        _textManager.load("data/dialog.dgl");
    }
}