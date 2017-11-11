#pragma once

#include <boost/container/flat_map.hpp>

#include "PageSection.hpp"

namespace rat {
	template<typename TKey, typename... TAssets>
	class AssetsManager {
	public:

		using Key_t       = TKey;
		using Section_t   = rat::PageSection;
		template<typename T>
		using Container_t = boost::container::flat_map<Key_t, std::unique_ptr<T>>;
		template<typename T>
		using Hold_t      = std::tuple<Section_t, Container_t<T>>;
		using Holder_t    = std::tuple<Hold_t<TAssets>...>;

	private:

		Holder_t _holder;

		template<typename UType>
		Section_t& getSection();
		template<typename UType>
		const Section_t& getSection() const;

		template<typename UType>
		Container_t<UType>& getContainer();
		template<typename UType>
		const Container_t<UType>& getContainer() const;

	public:

		AssetsManager() = default;

		AssetsManager(const AssetsManager&) = delete;

		AssetsManager& operator = (const AssetsManager&) = delete;

		AssetsManager(AssetsManager&&) = default;

		AssetsManager& operator = (AssetsManager&&) = default;

		template<typename UType>
		void setSectionFor(Section_t&& section);

		template<typename UType, typename... UArgs>
		UType& emplace(const Key_t& key, UArgs&&... args);

		template<typename UType>
		void erase(const Key_t& key);

		template<typename UType>
		UType& get(const Key_t& key);
		template<typename UType>
		const UType& get(const Key_t& key) const;

		template<typename UType>
		bool exists(const Key_t& key) const;
	};
}

#include "AssetsManager.tpp"
