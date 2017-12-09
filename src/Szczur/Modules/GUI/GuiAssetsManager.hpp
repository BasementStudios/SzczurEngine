#pragma once

#include <variant>

#include <boost/container/flat_map.hpp>
#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Hash.hpp"

namespace rat {
    template<typename... Ts>
    class GuiAssetsManager {
    public:
        using Variant_t = std::variant<Ts...>;
        using Assets_t = boost::container::flat_map<Hash32_t, Variant_t*>;

        GuiAssetsManager();
        ~GuiAssetsManager();
        
        template<typename T>
        void loadFromFile(const std::string& path);

        template<typename T>
        T* get(const std::string& path) const;

    private:
        Assets_t _assets;
    };
}

#include "GuiAssetsManager.tpp"