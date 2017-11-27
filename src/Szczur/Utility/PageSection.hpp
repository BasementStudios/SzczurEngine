#pragma once

#include <cstddef>
#include <memory>

#include "Szczur/Debug/Assert.hpp"

namespace rat {
	class PageSection {
	public:

		using Byte_t    = std::byte;
		using Pointer_t = Byte_t*;
		using Size_t    = size_t;

	protected:

		Pointer_t _data;
		Size_t _size;
		Size_t _capacity;

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
		T* create(TArgs&&... args);

		template<typename T, typename... TArgs>
		T* createPOD(TArgs&&... args);

		PageSection createSection(Size_t size);

		Pointer_t getData() const;

		template<typename T>
		T* getDataAs() const;

		Size_t getSize() const;

		template<typename T>
		Size_t getSizeFor() const;

		Size_t getCapacity() const;

		template<typename T>
		Size_t getCapacityFor() const;

		bool isEmpty() const;

		bool isFull() const;
	};
}

#include "PageSection.tpp"
