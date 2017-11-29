namespace rat {
	template<typename T, typename... TArgs>
	T* PageSection::create(TArgs&&... args) {
		rat_Assert(_data != nullptr && _size + sizeof(T) <= _capacity);

		T* tmp = new(_data + _size) T(std::forward<TArgs>(args)...);

		_size += sizeof(T);

		return tmp;
	}

	template<typename T, typename... TArgs>
	T* PageSection::createPOD(TArgs&&... args) {
		rat_Assert(_data != nullptr && _size + sizeof(T) <= _capacity);

		T* tmp = reinterpret_cast<T*>(_data + _size);

		*tmp = T(std::forward<TArgs>(args)...);

		_size += sizeof(T);

		return tmp;
	}

	template<typename T>
	T* PageSection::getDataAs() const {
		return reinterpret_cast<T*>(_data);
	}

	template<typename T>
	PageSection::Size_t PageSection::getSizeFor() const {
		return _size / sizeof(T);
	}

	template<typename T>
	PageSection::Size_t PageSection::getCapacityFor() const {
		return _capacity / sizeof(T);
	}
}
