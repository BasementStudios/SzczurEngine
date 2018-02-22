#include "CircleChooseWidget.hpp"
namespace rat {
    template<typename Tuple>
    GUI::GUI(Tuple&& tuple) :
    Module(tuple) {
        _initAssets();
        auto* a = reinterpret_cast<CircleChooseWidget*>(addInterface("data/json.json")->get("_root")->get("test"));
        a->setAmount(7u);
    }

    template<typename T>
    T* GUI::getAsset(const std::string& key) const {
        return _assets.get<T>(key);
    }

    template<typename T>
    void GUI::addAsset(const std::string& path) {
        _assets.loadFromFile<T>(path);
    }
}