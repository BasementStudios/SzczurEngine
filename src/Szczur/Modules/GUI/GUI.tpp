namespace rat {
    template<typename T>
    _GUI<T>::_GUI() {
        
        _assets.loadFromFile<sf::Texture>("data/button.png");
        _assets.loadFromFile<sf::Texture>("data/button-active.png");
        _assets.loadFromFile<sf::Texture>("data/button-clicked.png");
        _assets.loadFromFile<sf::Texture>("data/check.png");
        _assets.loadFromFile<sf::Texture>("data/check-on.png");
        _assets.loadFromFile<sf::Font>("data/consolab.ttf");
        _assets.loadFromFile<sf::Texture>("data/button.png");
        _assets.loadFromFile<Json>("data/json.json");
        //_guiJson.init(_assets.get<Json>("data/json.json"), &_assets, &_root, getModule<Window>().getSize());
        getModule<Window>();
    }

    
    template<typename T>
    void _GUI<T>::input(const sf::Event& event) {
        if(
            event.type == sf::Event::MouseButtonPressed || 
            event.type == sf::Event::MouseButtonReleased || 
            event.type == sf::Event::MouseMoved ||
            event.type == sf::Event::TextEntered
        ) {
            _root.input(event);
        }
    }

    template<typename T>
    void _GUI<T>::update(float deltaTime) {
        _root.update(deltaTime);
    }

    template<typename T>
    void _GUI<T>::render() {
        getModule<Window>().getWindow().draw(_root);
    }

    template<typename T>
    void _GUI<T>::reload() {
        _guiJson.reload(getModule<Window>().getSize(), &_root);
    }
}