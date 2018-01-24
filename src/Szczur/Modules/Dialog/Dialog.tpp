namespace rat {
    template<typename Tuple>
    Dialog::Dialog(Tuple&& tuple, const std::string& file) :
    Module(tuple) {
        auto& gui = getModule<GUI>();

        gui.addAsset<Json>(file);

        _interface = gui.addInterface(file);

        _dialogManager.selectDialog("1");
    
        _area = new TextAreaWidget;
        _area->setString(*_dialogManager.next());
        _area->setSize({25u, 2u});
        _area->setCharacterSize(28u);
        _area->setFont(gui.getAsset<sf::Font>("data/consolab.ttf"));
        _area->setColor(sf::Color(255,0,255));        

        _interface->add(_area);

        TextWidget* button = new TextWidget;
        button->setFont(gui.getAsset<sf::Font>("data/consolab.ttf"));
        button->setColor(sf::Color::Red);
        button->move({500.f, 0.f});
        button->setString("Next");
        button->setCallback(Widget::CallbackType::onRelease, [this](Widget*){
            _area->setString(*_dialogManager.next());
        });
        _interface->add(button);

        
    }
}