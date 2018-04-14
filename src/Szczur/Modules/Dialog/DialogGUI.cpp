#include "DialogGUI.hpp"

#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

namespace rat {

    DialogGUI::DialogGUI(GUI& gui) :
    _character(nullptr) {
        
        
    }

    DialogGUI::~DialogGUI() {
        //@TODO: remove _interface from _gui
    }

    void DialogGUI::hide() {
        _interface->deactivate();
        _interface->invisible();
    }

    void DialogGUI::show() {
        _interface->activate();
        _interface->visible();
    }

    void DialogGUI::initScript(Script& script) {
        auto object = script.newClass<DialogGUI>("DialogGUI", "Dialog");
        object.setProperty(
            "set",
            [](){},
            [](DialogGUI& owner, sol::table tab){
                if(tab["area"].valid() && tab["container"].valid() && 
                tab["character"].valid() && tab["name"].valid() &&
                tab["creator"].valid() && tab["interface"].valid()) {
					owner.setArea(tab["area"]);
                    owner.setButtonsContainer(tab["container"]);
                    owner.setCharacterHolder(tab["character"]);
                    owner.setName(tab["name"]);
                    owner.setButtonsCreator(tab["creator"]);
                    owner.setInterface(tab["interface"]);
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

    void DialogGUI::setInterface(Widget* _interface) {
        this->_interface = _interface;
    }

    Widget* DialogGUI::getInterface() const {
        return _interface;
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

    
    void DialogGUI::setKillerCallback(const std::function<void()>& killerCallback) {
        _killerCallback = killerCallback;
    }

    const std::function<void()>& DialogGUI::getKillerCallback() const {
        return _killerCallback;
    }

    void DialogGUI::interpretOptions(TextManager& textManager, Options& options, std::function<void(size_t)> callback) {
        size_t i = 0u;
        _area->invisible();
        options.forEach([&i, this, callback, &textManager](Options::Option* option){
            if(option->condition == nullptr || std::invoke(option->condition)) {
                TextWidget* button = new TextWidget;
                //button->setString(textManager.getLabel(option->target));
                _buttonsCreator.call(
                    i, 
                    button
                );
                button->setString(textManager.getLabel(option->target));
                if(option->finishing) {
                    button->setCallback(Widget::CallbackType::onRelease, [this, option, callback](Widget*){
                        if(option->afterAction)
                            std::invoke(option->afterAction);
                        std::invoke(_killerCallback);
                    });
                }
                
                else {
                    button->setCallback(Widget::CallbackType::onRelease, [this, option, callback](Widget*){
                            if(option->afterAction)
                                std::invoke(option->afterAction);
                            std::invoke(callback, option->target);
                    });
                }
                
                _buttonsContainer->add(button);
                ++i;
            }
        });
    }
}