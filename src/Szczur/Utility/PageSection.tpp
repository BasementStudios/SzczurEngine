namespace rat {
	template<typename T, typename... TArgs>
	T* PageSection::create(TArgs&&... _Args) {
		assert(_data != nullptr && _offset + sizeof(T) <= _size);

		T* tmp = new(_data + _offset) T(std::forward<TArgs>(_Args)...);

		advanceOffset(sizeof(T));

		return tmp;
	}
}
