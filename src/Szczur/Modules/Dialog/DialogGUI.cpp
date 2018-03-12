#include "DialogGUI.hpp"

namespace rat {

    DialogGUI::DialogGUI(GUI& gui):
    _gui(gui) {
        
        
    }

    DialogGUI::~DialogGUI() {
        //@TODO: remove _interface from _gui
    }

    void DialogGUI::clear() {
        _buttons->clear();
    }

    void DialogGUI::create() {
        _interface = _gui.addInterface("data/dialog.json");
        
        _area = reinterpret_cast<TextAreaWidget*>(
            _interface->get("_root")->get("background")->get("area")
        );

        _buttons = _interface->get("_root")->get("buttons");
    }

    void DialogGUI::interpretOptions(Options& options, std::function<void(size_t)> callback) {
        int i = 0;
        options.forEach([&i, this, callback](Options::Option* option){
            if(option->condition == nullptr || std::invoke(option->condition)) {
                TextWidget* button = new TextWidget;
                button->move({0.f, 50.f*i - 300.f});
                button->setFont(_gui.getAsset<sf::Font>("data/consolab.ttf"));
                button->setString(option->name);
                button->setCharacterSize(30u);
                button->setCallback(Widget::CallbackType::onRelease, [this, option, callback](Widget*){
                        if(option->afterAction)
                            std::invoke(option->afterAction);
                        std::invoke(callback, option->target);
                });
                ++i;
                _buttons->add("button"+std::to_string(i), button);
                
            }
        });
    }

    void DialogGUI::setText(const std::string& text) {
        _area->setString(text);
    }
}