#include "PageSection.hpp"

namespace rat {
	void PageSection::advanceOffset(Size_t offset) {
		_offset += offset;
	}

	void PageSection::clear() {
		_data = nullptr;
		_offset = 0;
		_size = 0;
	}

	PageSection::PageSection() :
		_data(nullptr), _offset(0), _size(0) {

	}

	PageSection::PageSection(Pointer_t data, Size_t size) :
		_data(data), _offset(0), _size(size) {

	}

	PageSection::PageSection(PageSection&& other) noexcept :
		_data(other._data), _offset(other._offset), _size(other._size) {
		other.clear();
	}

	PageSection& PageSection::operator = (PageSection&& other) noexcept {
		_data = other._data;
		_offset = other._offset;
		_size = other._size;

		other.clear();

		return *this;
	}

	PageSection PageSection::createSection(Size_t size) {
		assert(_data != nullptr && _offset + size <= _size);

		PageSection tmp(_data + _offset, size);

		advanceOffset(size);

		return tmp;
	}

	PageSection::Pointer_t PageSection::getData() const {
		return _data;
	}

	PageSection::Size_t PageSection::getSize() const {
		return _size;
	}
}
