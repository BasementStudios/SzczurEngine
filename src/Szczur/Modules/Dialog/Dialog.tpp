
#include "Szczur/Debug/Logger.hpp"
namespace rat {
    template<typename Tuple>
    Dialog::Dialog(Tuple&& tuple, const std::string& file) :
    Module(tuple) {
        
        auto& gui = getModule<GUI>();
        gui.addAsset<Json>(file);
        gui.addAsset<sf::Texture>("data/dial_back.png");

        _interface = gui.addInterface(file);
        TextAreaWidget* area = reinterpret_cast<TextAreaWidget*>(_interface->get("_root")->get("background")->get("area"));

        _dialogManager.load("data/dialog");
       // area->setString(_dialogManager.play(2u));

        _interface->get("_root")->get("button1")->setCallback(Widget::CallbackType::onRelease, [this, area](Widget*){
            area->setString(_dialogManager.play(1u));
        });
        _interface->get("_root")->get("button2")->setCallback(Widget::CallbackType::onRelease, [this, area](Widget*){
            area->setString(_dialogManager.play(2u));
        });
        _interface->get("_root")->get("button3")->setCallback(Widget::CallbackType::onRelease, [this, area](Widget*){
            area->setString(_dialogManager.play(3u));
        });
    }
}