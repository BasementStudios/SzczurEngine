#include "DialogGUI.hpp"

namespace rat {

    DialogGUI::DialogGUI(GUI& gui) :
    _character(nullptr) {
        
        
    }

    DialogGUI::~DialogGUI() {
        //@TODO: remove _interface from _gui
    }

    void DialogGUI::initScript(Script& script) {
        auto object = script.newClass<DialogGUI>("DialogGUI", "Dialog");
        object.setProperty(
            "set",
            [](){},
            [](DialogGUI& owner, sol::table tab){
                if(tab["area"].valid() && tab["container"].valid() && 
                tab["character"].valid() && tab["name"].valid() &&
                tab["creator"].valid()) {
                    owner.setArea(tab["area"]);
                    owner.setButtonsContainer(tab["container"]);
                    owner.setCharacterHolder(tab["character"]);
                    owner.setName(tab["name"]);
                    owner.setButtonsCreator(tab["creator"]);
                }
            }
        );
        object.init();
    }

    void DialogGUI::clear() {
        _buttonsContainer->clear();
    }

    void DialogGUI::setText(const std::string& text) {
        _area->setString(text);
        _area->visible();
    }

    void DialogGUI::setCharacterTexture(sf::Texture* texture) {
        if(_character == nullptr) {
            _character = new ImageWidget;
            
            _characterHolder->add(_character);
        }
        _character->setTexture(texture);
    }

    void DialogGUI::setCharacterName(const std::string& name) {
        _name->setString(name);
    }


    void DialogGUI::setName(TextWidget* name) {
        _name = name;
    }

    TextWidget* DialogGUI::getName() const {
        return _name;
    }
    
    void DialogGUI::setCharacterHolder(Widget* holder) {
        _characterHolder = holder;
    }

    Widget* DialogGUI::getCharacterHolder() const {
        return _characterHolder;
    }

    void DialogGUI::setArea(TextAreaWidget* area) {
        _area = area;
    }

    TextAreaWidget* DialogGUI::getArea() const {
        return _area;
    }   

    void DialogGUI::setButtonsCreator(const sol::function& func) {
        _buttonsCreator = func;
    }

    Widget* DialogGUI::getButtonsCreator() const {
        return _buttonsContainer;
    }

    void DialogGUI::setButtonsContainer(Widget* container) {
        _buttonsContainer = container;
    }

    const sol::function& DialogGUI::getButtonsContainer() const {
        return _buttonsCreator;
    }

    void DialogGUI::interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t)> callback) {
        size_t i = 0u;
        _area->invisible();
        options.forEach([&i, this, callback, &textManager](Options::Option* option){
            if(option->condition == nullptr || std::invoke(option->condition)) {
                TextWidget* button = new TextWidget;
                button->setString(textManager.getLabel(option->target));
                _buttonsCreator.call(
                    i, 
                    button
                );
                button->setString(textManager.getLabel(option->target));
                button->setCallback(Widget::CallbackType::onRelease, [this, option, callback](Widget*){
                        if(option->afterAction)
                            std::invoke(option->afterAction);
                        std::invoke(callback, option->target);
                });
                _buttonsContainer->add(button);
                ++i;
                /*button->move({0.f, 30.f*i});
                
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
                _buttons->add(button);
                */
            }
        });
    }
}