namespace rat {
	template<typename TKey, typename... TAssets>
	template<typename UType>
	typename AssetsManager<TKey, TAssets...>::Section_t& AssetsManager<TKey, TAssets...>::getSection() {
		return std::get<Section_t>(std::get<Hold_t<UType>>(_holder));
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	const typename AssetsManager<TKey, TAssets...>::Section_t& AssetsManager<TKey, TAssets...>::getSection() const {
		return std::get<Section_t>(std::get<Hold_t<UType>>(_holder));
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	typename AssetsManager<TKey, TAssets...>::template Container_t<UType>& AssetsManager<TKey, TAssets...>::getContainer() {
		return std::get<Container_t<UType>>(std::get<Hold_t<UType>>(_holder));
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	const typename AssetsManager<TKey, TAssets...>::template Container_t<UType>& AssetsManager<TKey, TAssets...>::getContainer() const {
		return std::get<Container_t<UType>>(std::get<Hold_t<UType>>(_holder));
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	void AssetsManager<TKey, TAssets...>::setSectionFor(Section_t&& section) {
		getSection<UType>() = std::move(section);
	}

	template<typename TKey, typename... TAssets>
	template<typename UType, typename... UArgs>
	UType& AssetsManager<TKey, TAssets...>::emplace(const Key_t& key, UArgs&&... args) {
		return *std::get<1>(*std::get<0>(getContainer<UType>().emplace(key, getSection<UType>().template create<UType>(std::forward<UArgs>(args)...))));
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	void AssetsManager<TKey, TAssets...>::erase(const Key_t& key) {
		getContainer<UType>().erase(key);
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	UType& AssetsManager<TKey, TAssets...>::get(const Key_t& key) {
		return *getContainer<UType>().at(key);
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	const UType& AssetsManager<TKey, TAssets...>::get(const Key_t& key) const {
		return *getContainer<UType>().at(key);
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	UType* AssetsManager<TKey, TAssets...>::getPtr(const Key_t& key) {
		return getContainer<UType>().at(key).get();
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	const UType* AssetsManager<TKey, TAssets...>::getPtr(const Key_t& key) const {
		return getContainer<UType>().at(key).get();
	}

	template<typename TKey, typename... TAssets>
	template<typename UType>
	bool AssetsManager<TKey, TAssets...>::exists(const Key_t& key) const {
		return getContainer<UType>().count(key) != 0;
	}
}
