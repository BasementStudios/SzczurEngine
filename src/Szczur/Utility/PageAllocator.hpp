#pragma once

#include "PageSection.hpp"

namespace rat {
	class PageAllocator : public PageSection {
	public:

		PageAllocator() = default;

		PageAllocator(const PageAllocator&) = delete;

		PageAllocator& operator = (const PageAllocator&) = delete;

		PageAllocator(PageAllocator&& _Other) noexcept = default;

		PageAllocator& operator = (PageAllocator&& _Other) noexcept = default;

		~PageAllocator() override;

		bool allocate(Size_t size);

		void deallocate();

		bool isAllocated() const;
	};
}
