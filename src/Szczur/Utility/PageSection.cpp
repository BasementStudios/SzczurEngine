#include "PageSection.hpp"

namespace rat {
	void PageSection::clear() {
		_data = nullptr;
		_size = 0;
		_capacity = 0;
	}

	PageSection::PageSection() :
		_data(nullptr), _size(0), _capacity(0) {

	}

	PageSection::PageSection(Pointer_t data, Size_t size) :
		_data(data), _size(0), _capacity(size) {

	}

	PageSection::PageSection(PageSection&& other) noexcept :
		_data(other._data), _size(other._size), _capacity(other._capacity) {
		other.clear();
	}

	PageSection& PageSection::operator = (PageSection&& other) noexcept {
		_data = other._data;
		_size = other._size;
		_capacity = other._capacity;

		other.clear();

		return *this;
	}

	PageSection PageSection::createSection(Size_t size) {
		assert(_data != nullptr && _size + size <= _capacity);

		PageSection tmp(_data + _size, size);

		_size += size;

		return tmp;
	}

	PageSection::Pointer_t PageSection::getData() const {
		return _data;
	}

	PageSection::Size_t PageSection::getSize() const {
		return _size;
	}

	PageSection::Size_t PageSection::getCapacity() const {
		return _capacity;
	}
}
