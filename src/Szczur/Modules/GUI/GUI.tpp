namespace rat {
    template<typename T>
    T* GUI::getAsset(const std::string& key) const {
        return _assets.get<T>(key);
    }
    template<typename T>
    void GUI::addAsset(const std::string& path) {
        _assets.loadFromFile<T>(path);
    }
}