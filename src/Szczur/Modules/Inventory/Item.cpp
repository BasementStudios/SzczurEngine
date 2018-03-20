#include "Item.hpp"
#include <iostream>
namespace rat {
    Item::Item(Widget* owner):
    _owner(owner) {
    }

    Item::~Item() {
        if(_owner)
            _owner->clear();
    }

    void Item::initScript(Script& script) {
        auto object = script.newClass<Item>("Item", "Inventory");

        object.set("setTexture", &Item::setTexture);

        object.setProperty(
            "description",
            [](){},
            [](Item& owner, sol::table tab){ 
                if(tab["text"].valid() && tab["texture"].valid() && tab["font"].valid() && tab["fontSize"].valid()) {
                    owner.setDescription(tab["text"], tab["texture"], tab["font"], tab["fontSize"]);
                }
            }
        );

        object.init();
    }

    void Item::setDescription(const std::string& text, sf::Texture* texture, sf::Font* font, size_t fontSize) {
        if(!_description && _sprite) {
            _description = new ImageWidget(texture);
            _description->invisible();
            _description->deactivate();
            _sprite->add(_description);

            _sprite->setCallback(Widget::CallbackType::onHoverIn, [this](Widget*){
                _description->activate();
                _description->visible();
            });

            _description->setCallback(Widget::CallbackType::onHoverOut, [](Widget* owner){
                owner->invisible();
                owner->deactivate();
            });

            ScrollAreaWidget* area = new ScrollAreaWidget;
            _description->add(area);
            area->setSize(260, 160);
            area->move(20,20);

            TextAreaWidget* textArea = new TextAreaWidget;
            textArea->setSize({10,2});
            textArea->setString(text);
            textArea->setFont(font);
            textArea->setCharacterSize(fontSize);
            textArea->setColor(sf::Color::White);
            area->add(textArea);
        }
    }

    void Item::setTexture(sf::Texture* texture) {
        if(!_sprite) {
            _sprite = new ImageWidget;
            _owner->add(_sprite);
        }
        _sprite->setTexture(texture);
    }
}