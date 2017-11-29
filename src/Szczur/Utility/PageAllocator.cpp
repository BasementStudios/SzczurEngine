#include "PageAllocator.hpp"

namespace rat {
	PageAllocator::~PageAllocator() {
		deallocate();
	}

	bool PageAllocator::allocate(Size_t size) {
		_data = static_cast<Pointer_t>(::operator new(size, std::nothrow));

		if(isAllocated()) {
			_size = 0;
			_capacity = size;

			return true;
		}

		return false;
	}

	void PageAllocator::deallocate() {
		if(isAllocated()) {
			::operator delete(_data);
		}
	}

	bool PageAllocator::isAllocated() const {
		return _data != nullptr;
	}
}
