#pragma once

#include <cassert>
#include <memory>

namespace rat {
	class BlockAllocator {
	public:

		using Byte_t    = unsigned char;
		using Pointer_t = Byte_t*;
		using Size_t    = size_t;

	private:

		Pointer_t _data;
		Size_t _offset;
		Size_t _capacity;

	public:

		BlockAllocator() :
			_data(nullptr), _offset(0), _capacity(0) {}

		BlockAllocator(const BlockAllocator&) = delete;

		BlockAllocator& operator = (const BlockAllocator&) = delete;

		BlockAllocator(BlockAllocator&& other) noexcept :
	        _data(other._data), _offset(other._offset), _capacity(other._capacity) {
	        other.clear();
	    }

	    BlockAllocator& operator = (BlockAllocator&& other) noexcept {
	        _data = other._data;
	        _offset = other._offset;
	        _capacity = other._capacity;

	        other.clear();

	        return *this;
	    }

		~BlockAllocator() {
			deallocate();
		}

		bool allocate(Size_t sizeInBytes) {
			_data = static_cast<Pointer_t>(::operator new(sizeInBytes, std::nothrow));

			if(isAllocated()) {
				_capacity = sizeInBytes;
				return true;
			}

			return false;
		}

		void deallocate() {
			if(isAllocated()) {
				::operator delete(_data);

				_data = nullptr;
				_offset = 0;
				_capacity = 0;
			}
		}

		bool isAllocated() const {
			return _data != nullptr;
		}

		template <typename T, typename... Ts>
		T* create(Ts&&... args) {
			assert(_offset + sizeof(T) <= _capacity);

			auto tmp = new(getShiftedData()) T(std::forward<Ts>(args)...);

			advanceOffset(sizeof(T));

			return tmp;
		}

		void advanceOffset(Size_t offset) {
			_offset += offset;
		}

		Pointer_t getData() const {
			return _data;
		}

		Size_t getOffset() const {
			return _offset;
		}

		Pointer_t getShiftedData() const {
			return getData() + getOffset();
		}

		Size_t getCapacity() const {
			return _capacity;
		}

	private:

		void clear() {
	        _data = nullptr;
	        _offset = 0;
	        _capacity = 0;
    	}
	};
}
