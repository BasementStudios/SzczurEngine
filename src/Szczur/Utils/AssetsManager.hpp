#pragma once

#include "BlockAllocator.hpp"
#include "Hash.hpp"
#include "HashVector.hpp"

namespace rat {
	template<typename K, typename... Ts>
	class AssetsManager {
	public:

		using Key_t       = K;
		template<typename T>
		using Container_t = rat::HashVector<Key_t, std::unique_ptr<T>>;
		using Holder_t    = std::tuple<Container_t<Ts>...>;
		using Allocator_t = rat::BlockAllocator;

	private:

		Allocator_t _alloc;
		Holder_t _holder;

	public:

		AssetsManager() = default;

		AssetsManager(const AssetsManager&) = delete;

		AssetsManager& operator = (const AssetsManager&) = delete;

		template <typename U, typename... Us>
		U& add(Key_t key, Us&&... args) {
			return *getContainer<U>().emplace(key, _alloc.create<U>(std::forward<Us>(args)...));
		}

		template <typename U>
		U& get(Key_t key) {
			return *getContainer<U>().get(key);
		}

		template <typename U>
		const U& get(Key_t key) const {
			return *getContainer<U>().get(key);
		}

		Allocator_t& getAllocator() {
			return _alloc;
		}

		const Allocator_t& getAllocator() const {
			return _alloc;
		}

		template <typename U>
		Container_t<U>& getContainer() {
			return std::get<Container_t<U>>(_holder);
		}

		template <typename U>
		const Container_t<U>& getContainer() const {
			return std::get<Container_t<U>>(_holder);
		}

		template <typename U>
		void prepareContainer() {
			getContainer<U>().sort();
		}
	};
}
