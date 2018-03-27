namespace rat {
    template<typename... Ts>
    GuiAssetsManager<Ts...>::GuiAssetsManager() {

    }

    template<typename... Ts>
    GuiAssetsManager<Ts...>::~GuiAssetsManager() {
        for(auto& it : _assets)
            delete it.second;
    }

    template<typename... Ts>
    template<typename T>
    void GuiAssetsManager<Ts...>::loadFromFile(const std::string& path) {
        T obj;
        if(obj.loadFromFile(path)){
            Variant_t* var = new Variant_t;
            *var = obj;
            _assets.insert_or_assign(fnv1a_32(path.begin(), path.end()), var);
        }
        else {
            LOG_ERROR("Cannot load file: \"", path, "\"")
        }
    }

    template<typename... Ts>
    template<typename T>
    T* GuiAssetsManager<Ts...>::get(const std::string& path) const {
        if(auto it = _assets.find(fnv1a_32(path.begin(), path.end())); it != _assets.end())
            return &(std::get<T>(*(it->second)));
        LOG_ERROR("Cannot find file: \"", path, "\"")
        return nullptr;
    }
}