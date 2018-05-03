#pragma once

#include <variant>
#include <fstream>
#include <boost/container/flat_map.hpp>
#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

namespace rat {
    template<template<typename...> typename T, typename... Ts, typename F, size_t... Is>
    void forEachImpl(T<Ts...>& obj, F&& call, std::index_sequence<Is...>) {
        ((call(std::get<Is>(obj))), ...);
    }

    template<template<typename...> typename T, typename... Ts, typename F>
    void forEach(T<Ts...>& obj, F&& call) {
        forEachImpl(obj, call, std::index_sequence_for<Ts...>());
    }

    template<typename... Ts>
    class GuiAssetsManager {
    public:
        using Key_t = size_t;
        template<typename T>
        using Container_t = std::map<Key_t, T*>;
        
        ~GuiAssetsManager() {
            forEach(tuple, [](auto& obj){
                for(auto& it : obj)
                    delete it.second;
            });
        }
        
        template<typename T>
        T* loadFromFile(const std::string& path)
        {
            T* obj = new T;
            if(obj->loadFromFile(path))
            {
                _add( fnv1a_32(path.begin(), path.end()), obj);
                return obj;
            }
            else 
            {
                LOG_ERROR("Cannot load file: \"", path, "\"");
                delete obj;
                return nullptr;
            }
        }

        template<typename T>
        T* get(const std::string& path)
        {
            auto* result = _get<T>(fnv1a_32(path.begin(), path.end()));
            if(!result) 
            {
                return loadFromFile<T>(path);
                //LOG_ERROR("Cannot get file: \"", path, "\"");
            }
            return result;
        }
        
        
    private:
        template<typename T>
        void _add(Key_t key, T* obj) {
            auto& temp = std::get<Container_t<T>>(tuple);
            if(auto it = temp.find(key); it != temp.end())
                delete it->second;
            temp[key] = obj;
        }

        template<typename T>
        T* _get(Key_t key) const {
            auto it = std::get<Container_t<T>>(tuple).find(key);
            if(it != std::get<Container_t<T>>(tuple).end())
                return it->second;
            return nullptr;
        }

        std::tuple<Container_t<Ts>...> tuple;
    };

    using BasicGuiAssetsManager = GuiAssetsManager<sf::Font, sf::Texture>;
}
