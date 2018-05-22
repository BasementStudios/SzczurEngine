#pragma once

#include <tuple>
#include <vector>

#include "AssetTraits.hpp"
#include "Szczur/Utility/Algorithm/BinarySearch.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

namespace rat
{

template <typename T, typename Allocator = std::allocator<std::tuple<Hash64_t, std::unique_ptr<T>>>>
class AssetManager
{
public:

    using Key_t                  = Hash64_t;
    using Underlying_t           = T;
    using Mapped_t               = std::unique_ptr<T>;
    using Container_t            = std::vector<std::tuple<Hash64_t, std::unique_ptr<T>>, Allocator>;
    using Compare_t              = std::less<Hash64_t>;
    using Allocator_t            = typename Container_t::allocator_type;
    using Value_t                = typename Container_t::value_type;
    using Reference_t            = typename Container_t::reference;
    using ConstReference_t       = typename Container_t::const_reference;
    using Pointer_t              = typename Container_t::pointer;
    using ConstPointer_t         = typename Container_t::const_pointer;
    using Size_t                 = typename Container_t::size_type;
    using Iterator_t             = typename Container_t::iterator;
    using ConstIterator_t        = typename Container_t::const_iterator;
    using ReverseIterator_t      = typename Container_t::reverse_iterator;
    using ConstReverseIterator_t = typename Container_t::const_reverse_iterator;
    using Traits_t               = AssetTraits<T>;

    using OperationStruct_t      = struct
    {
        Value_t& getValue() { return *iterator; }
        Key_t& getKey() { return std::get<0>(*iterator); }
        Mapped_t& getMapped() { return std::get<1>(*iterator); }
        Underlying_t& getUnderlying() { return *std::get<1>(*iterator); }

        Iterator_t iterator;
        bool success;
    };

    using ConstOperationStruct_t  = struct
    {
        const Value_t& getValue() const { return *iterator; }
        const Key_t& getKey() const { return std::get<0>(*iterator); }
        const Mapped_t& getMapped() const { return std::get<1>(*iterator); }
        const Underlying_t& getUnderlying() const { return *std::get<1>(*iterator); }

        ConstIterator_t iterator;
        bool success;
    };

    ///
    AssetManager() = default;

    ///
    AssetManager(const AssetManager&) = default;

    ///
    AssetManager& operator = (const AssetManager&) = default;

    ///
    AssetManager(AssetManager&&) = default;

    ///
    AssetManager& operator = (AssetManager&&) = default;

    ///
    ~AssetManager() = default;

    ///
    void reserve(Size_t size)
    {
        _container.reserve(size);
    }

    ///
    template <typename K>
    Mapped_t& emplace(K&& key)
    {
        auto hint = getLowerBound(key);

        return std::get<1>(*_container.emplace(hint, std::forward<K>(key), Traits_t::create()));
    }

    ///
    template <typename K>
    OperationStruct_t tryEmplace(K&& key)
    {
        auto hint = find(key);

        if (hint == _container.end())
        {
            return { _container.emplace(getLowerBound(key), std::forward<K>(key), Traits_t::create()), true };
        }

        return { hint, false };
    }

    ///
    Size_t erase(const std::string& path)
    {
        return erase(_getKeyFromPath(path));
    }

    ///
    template <typename K>
    Size_t erase(K&& key)
    {
        Size_t count = 0u;

        while (true) // TODO erase with std::remove_if
        {
            if (auto it = find(key); it != _container.end())
            {
                _container.erase(it);
                ++count;
            }
            else
            {
                break;
            }
        }

        return count;
    }

    ///
    void erase(ConstIterator_t iterator)
    {
        _container.erase(iterator);
    }

    ///
    void clear()
    {
        _container.clear();
    }

    ///
    Iterator_t getLowerBound(const std::string& path)
    {
        return getLowerBound(_getKeyFromPath(path));
    }

    ///
    ConstIterator_t getLowerBound(const std::string& path) const
    {
        return getLowerBound(_getKeyFromPath(path));
    }

    ///
    template <typename K>
    Iterator_t getLowerBound(K&& key)
    {
        return lowerBound(_container.begin(), _container.end(), key, [](const auto& first, const auto& second) {
            return Compare_t{}(std::get<0>(first), second);
        });
    }

    ///
    template <typename K>
    ConstIterator_t getLowerBound(K&& key) const
    {
        return lowerBound(_container.begin(), _container.end(), key, [](const auto& first, const auto& second) {
            return Compare_t{}(std::get<0>(first), second);
        });
    }

    ///
    Iterator_t find(const std::string& path)
    {
        return find(_getKeyFromPath(path));
    }

    ///
    ConstIterator_t find(const std::string& path) const
    {
        return find(_getKeyFromPath(path));
    }

    ///
    template <typename K>
    Iterator_t find(K&& key)
    {
        return binarySearch(_container.begin(), _container.end(), key, [](const auto& first, const auto& second) {
            if constexpr (std::is_same_v<std::decay_t<decltype(second)>, std::decay_t<K>>)
            {
                return Compare_t{}(std::get<0>(first), second);
            }
            else
            {
                return Compare_t{}(first, std::get<0>(second));
            }
        });
    }

    ///
    template <typename K>
    ConstIterator_t find(K&& key) const
    {
        return binarySearch(_container.begin(), _container.end(), key, [](const auto& first, const auto& second) {
            if constexpr (std::is_same_v<std::decay_t<decltype(second)>, std::decay_t<K>>)
            {
                return Compare_t{}(std::get<0>(first), second);
            }
            else
            {
                return Compare_t{}(first, std::get<0>(second));
            }
        });
    }

    ///
    Underlying_t& getPtr(const std::string& path)
    {
        auto key = _getKeyFromPath(path);

        if (auto it = find(key); it != _container.end())
        {
            return std::get<1>(*it).get();
        }

        auto hint = getLowerBound(key);

        auto ptr = Traits_t::create();

        if (Traits_t::loadFromFile(*ptr, path))
        {
            return std::get<1>(*_container.emplace(hint, key, ptr)).get();
        }
        else
        {
            return nullptr;
        }
    }

    ///
    const Underlying_t& getPtr(const std::string& path) const
    {
        auto key = _getKeyFromPath(path);

        if (auto it = find(key); it != _container.end())
        {
            return std::get<1>(*it).get();
        }

        auto hint = getLowerBound(key);

        auto ptr = Traits_t::create();

        if (Traits_t::loadFromFile(*ptr, path))
        {
            return std::get<1>(*_container.emplace(hint, key, ptr)).get();
        }
        else
        {
            return nullptr;
        }
    }

    ///
    template <typename K>
    Underlying_t* getPtr(K&& key)
    {
        if (auto it = find(key); it != _container.end())
        {
            return std::get<1>(*it).get();
        }

        return nullptr;
    }

    ///
    template <typename K>
    const Underlying_t* getPtr(K&& key) const
    {
        if (auto it = find(key); it != _container.end())
        {
            return std::get<1>(*it).get();
        }

        return nullptr;
    }

    ///
    Underlying_t& get(const std::string& path)
    {
        Key_t key = _getKeyFromPath(path);

        if (auto it = find(key); it != _container.end())
        {
            return std::get<1>(*it->get());
        }

        auto hint = getLowerBound(key);

        auto ptr = Traits_t::create();

        Traits_t::loadFromFile(*ptr, path); // TODO add false handling

        return std::get<1>(*_container.emplace(hint, key, ptr));
    }

    ///
    const Underlying_t& get(const std::string& path) const
    {
        Key_t key = _getKeyFromPath(path);

        if (auto it = find(key); it != _container.end())
        {
            return std::get<1>(*it->get());
        }

        auto hint = getLowerBound(key);

        auto ptr = Traits_t::create();

        Traits_t::loadFromFile(*ptr, path); // TODO add false handling

        return std::get<1>(*_container.emplace(hint, key, ptr));
    }

    ///
    template <typename K>
    Underlying_t& get(K&& key)
    {
        return std::get<1>(*find(key)->get());
    }

    ///
    template <typename K>
    const Underlying_t& get(K&& key) const
    {
        return std::get<1>(*find(key)->get());
    }

    ///
    template <typename K>
    bool exists(K&& key) const
    {
        return find(key) != _container.end();
    }

    ///
    bool isEmpty() const
    {
        return _container.empty();
    }

    ///
    const Value_t* getData() const
    {
        return _container.data();
    }

    ///
    Size_t getSize() const
    {
        return _container.size();
    }

    ///
    Size_t getMaxSize() const
    {
        return _container.max_size();
    }

    ///
    Size_t getCapacity() const
    {
        return _container.capacity();
    }

    ///
    Allocator_t getAllocator() const
    {
        return _container.get_allocator();
    }

private:

    ///
    Key_t _getKeyFromPath(const std::string& path) const
    {
        return fnv1a_64(path.begin(), path.end());
    }

    Container_t _container;

};

}
