#pragma once

#include <memory>
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

	using Finder_t                = struct
	{
		template <typename U1, typename U2>
		auto operator () (const U1& first, const U2& second)
		{
			return Compare_t{}(std::get<0>(first), second);
		}

	};

	using ExtFinder_t             = struct
	{
		template <typename U1, typename U2>
		auto operator () (const U1& first, const U2& second)
		{
			if constexpr (std::is_same_v<std::decay_t<U2>, Key_t>)
			{
				return Compare_t{}(std::get<0>(first), second);
			}
			else
			{
				return Compare_t{}(first, std::get<0>(second));
			}
		}

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
	Mapped_t& emplace(Key_t key)
	{
		auto hint = getLowerBound(key);

		return std::get<1>(*_container.emplace(hint, key, Traits_t::create()));
	}

	///
	OperationStruct_t tryEmplace(Key_t key)
	{
		auto hint = find(key);

		if (hint == _container.end())
		{
			return { _container.emplace(getLowerBound(key), key, Traits_t::create()), true };
		}

		return { hint, false };
	}

	///
	Size_t erase(const std::string& path)
	{
		return erase(_getKeyFromPath(path));
	}

	///
	Size_t erase(Key_t key)
	{
		Size_t count = 0u;

		_container.erase(std::remove_if(_container.begin(), _container.end(), [key, &count](const auto& value) {
			if (std::get<0>(value) == key)
			{
				++count;

				return true;
			}
			else
			{
				return false;
			}
		}), _container.end());

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
	Iterator_t getLowerBound(Key_t key)
	{
		return lowerBound(_container.begin(), _container.end(), key, Finder_t{});
	}

	///
	ConstIterator_t getLowerBound(Key_t key) const
	{
		return lowerBound(_container.begin(), _container.end(), key, Finder_t{});
	}

	///
	Iterator_t find(const std::string& path)
	{
		auto key = _getKeyFromPath(path);

		if (auto it = binarySearch(_container.begin(), _container.end(), key, ExtFinder_t{}); it != _container.end())
		{
			return it;
		}

		auto hint = getLowerBound(key);

		auto ptr = Traits_t::create();

		if (Traits_t::loadFromFile(*ptr, path))
		{
			return _container.emplace(hint, key, ptr);
		}
		else
		{
			return _container.end();
		}
	}

	///
	ConstIterator_t find(const std::string& path) const
	{
		auto key = _getKeyFromPath(path);

		if (auto it = binarySearch(_container.begin(), _container.end(), key, ExtFinder_t{}); it != _container.end())
		{
			return it;
		}

		auto hint = getLowerBound(key);

		auto ptr = Traits_t::create();

		if (Traits_t::loadFromFile(*ptr, path))
		{
			return _container.emplace(hint, key, ptr);
		}
		else
		{
			return _container.end();
		}
	}

	///
	Iterator_t find(Key_t key)
	{
		return binarySearch(_container.begin(), _container.end(), key, ExtFinder_t{});
	}

	///
	ConstIterator_t find(Key_t key) const
	{
		return binarySearch(_container.begin(), _container.end(), key, ExtFinder_t{});
	}

	///
	Underlying_t* getPtr(const std::string& path)
	{
		if (auto it = find(path); it != _container.end())
		{
			return std::get<1>(*it).get();
		}

		return nullptr;
	}

	///
	const Underlying_t* getPtr(const std::string& path) const
	{
		if (auto it = find(path); it != _container.end())
		{
			return std::get<1>(*it).get();
		}

		return nullptr;
	}

	///
	Underlying_t* getPtr(Key_t key)
	{
		if (auto it = find(key); it != _container.end())
		{
			return std::get<1>(*it).get();
		}

		return nullptr;
	}

	///
	const Underlying_t* getPtr(Key_t key) const
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
		return std::get<1>(*find(path)->get());
	}

	///
	const Underlying_t& get(const std::string& path) const
	{
		return std::get<1>(*find(path)->get());
	}

	///
	Underlying_t& get(Key_t key)
	{
		return std::get<1>(*find(key)->get());
	}

	///
	const Underlying_t& get(Key_t key) const
	{
		return std::get<1>(*find(key)->get());
	}

	///
	bool exists(const std::string& path) const
	{
		return exists(_getKeyFromPath(path));
	}

	///
	bool exists(Key_t key) const
	{
		return find(key) != _container.end();
	}

	///
	size_t count(const std::string& path) const
	{
		return count(_getKeyFromPath(path));
	}

	///
	size_t count(Key_t key) const
	{
		return std::count_if(_container.begin(), _container.end(), [key](const auto& value) { // TODO with lowerBound
			return std::get<0>(value) == key;
		});
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
