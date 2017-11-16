#pragma once

#include <cassert>
#include <cstddef>
#include <memory>

namespace rat {
	class PageSection {
	public:

		using Byte_t    = std::byte;
		using Pointer_t = Byte_t*;
		using Size_t    = size_t;

	protected:

		Pointer_t _data;
		Size_t _offset;
		Size_t _size;

		void advanceOffset(Size_t offset);

		void clear();

	public:

		PageSection();

		PageSection(Pointer_t data, Size_t size);

		PageSection(const PageSection&) = delete;

		PageSection& operator = (const PageSection&) = delete;

		PageSection(PageSection&& other) noexcept;

		PageSection& operator = (PageSection&& other) noexcept;

		virtual ~PageSection() = default;

		template<typename T, typename... TArgs>
		T* create(TArgs&&... _Args);

		PageSection createSection(Size_t size);

		Pointer_t getData() const;

		Size_t getSize() const;
	};
}

#include "PageSection.tpp"
