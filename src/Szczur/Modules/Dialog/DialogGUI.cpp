#include "DialogGUI.hpp"

namespace rat {

    DialogGUI::DialogGUI(GUI& gui):
    _gui(gui),
    _characterTexture(nullptr) {
        
        
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
            _interface->get("_root")->get("dialog")->get("background")->get("area")
        );

        _name = reinterpret_cast<TextWidget*>(
            _interface->get("_root")->get("dialog")->get("name")
        );

        _characterTextureHolder = _interface->get("_root")->get("dialog")->get("aaaaaimageHolder");

        _buttons = _interface->get("_root")->get("dialog")->get("background")->get("buttons");
        
    }

    void DialogGUI::interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t)> callback) {
        int i = 0;
        _area->invisible();
        options.forEach([&i, this, callback, &textManager](Options::Option* option){
            if(option->condition == nullptr || std::invoke(option->condition)) {
                TextWidget* button = new TextWidget;

                button->move({0.f, 30.f*i});
                
                button->setFont(_gui.getAsset<sf::Font>("data/consolab.ttf"));
                button->setString(textManager.getLabel(option->target));
                button->setCharacterSize(20u);
                button->setColor(sf::Color(200,180,200));
                button->setCallback(Widget::CallbackType::onRelease, [this, option, callback](Widget*){
                        if(option->afterAction)
                            std::invoke(option->afterAction);
                        std::invoke(callback, option->target);
                });
                button->setCallback(Widget::CallbackType::onHoverIn, [button](Widget*){
                    button->setColor(sf::Color(200,100,200));
                });
                button->setCallback(Widget::CallbackType::onHoverOut, [button](Widget*){
                    button->setColor(sf::Color(200,180,200));
                });
                ++i;
                _buttons->add("button"+std::to_string(i), button);
                
            }
        });
    }

    void DialogGUI::setText(const std::string& text) {
        _area->setString(text);
        _area->visible();
    }
    void DialogGUI::setCharacter(const std::string& name) {
        _name->setString(name);
    }
    void DialogGUI::setCharacterTexture(sf::Texture* texture) {
        if(_characterTexture == nullptr) {
            _characterTexture = new ImageWidget;
            
            _characterTextureHolder->add("image", _characterTexture);
        }
        _characterTexture->setScale({0.1, 0.1f});
        _characterTexture->setTexture(texture);
    }
}