namespace rat {
    template<typename Tuple>
    GUI::GUI(Tuple&& tuple) :
    Module(tuple) {
        _initAssets();
        addInterface("data/json.json");
    }

    template<typename T>
    T* GUI::getAsset(const std::string& key) const {
        return _assets.get<T>(key);
    }
}