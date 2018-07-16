#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "AssetTraits.hpp"
#include "Szczur/Utility/Algorithm/BinarySearch.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"

namespace rat
{

template <typename T>
class AssetManager
{
public:

	using Key_t                  = Hash64_t;
	using Underlying_t           = T;
	using MappedPtr_t            = std::shared_ptr<Underlying_t>;
	using Container_t            = std::vector<std::pair<Key_t, MappedPtr_t>>;
	using Compare_t              = std::less<Key_t>;
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
		Key_t& getKey() { return iterator->first; }
		MappedPtr_t getPtr() const { return iterator->second; }
		Underlying_t& getUnderlying() { return *iterator->second; }

		Iterator_t iterator;
		const bool success;

	};

	using ConstOperationStruct_t  = struct
	{
		const Key_t& getKey() const { return iterator->first; }
		MappedPtr_t getPtr() const { return iterator->second; }
		const Underlying_t& getUnderlying() const { return *iterator->second; }

		ConstIterator_t iterator;
		const bool success;

	};

	using Finder_t                = struct
	{
		template <typename U1, typename U2>
		auto operator () (const U1& a, const U2& b)
		{
			return Compare_t{}(a.first, b);
		}

	};

	using ExtFinder_t             = struct
	{
		template <typename U1, typename U2>
		auto operator () (const U1& a, const U2& b)
		{
			if constexpr (std::is_same_v<std::decay_t<U2>, Key_t>)
			{
				return Compare_t{}(a.first, b);
			}
			else
			{
				return Compare_t{}(a, b.first);
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
	OperationStruct_t emplace(const std::string& path)
	{
		return emplace(_getKeyFromPath(path));
	}

	///
	OperationStruct_t emplace(Key_t key)
	{
		if (auto it = find(key); it == _container.end())
		{
			auto hint = getLowerBound(key);

			return { _container.emplace(hint, key, Traits_t::create()), true };
		}
		else
		{
			return { it, false };
		}
	}

	///
	bool erase(const std::string& path)
	{
		return erase(_getKeyFromPath(path));
	}

	///
	bool erase(Key_t key)
	{
		if (auto it = find(key); it != _container.end())
		{
			_container.erase(it);

			return true;
		}

		return false;
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

		if (auto it = find(key); it != _container.end())
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
	MappedPtr_t getPtr(const std::string& path)
	{
		return find(path)->second;
	}

	///
	MappedPtr_t getPtr(Key_t key) const
	{
		return find(key)->second;
	}

	///
	Underlying_t& get(const std::string& path)
	{
		return *find(path)->second;
	}

	///
	Underlying_t& get(Key_t key)
	{
		return *find(key)->second;
	}

	///
	const Underlying_t& get(Key_t key) const
	{
		return *find(key)->second;
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
	Size_t getUseCount(const std::string& path) const
	{
		return getUseCount(_getKeyFromPath(path));
	}

	///
	Size_t getUseCount(Key_t key) const
	{
		if (auto it = find(key); it != _container.end())
		{
			return it->second.use_count();
		}

		return 0u;
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
