#pragma once

#include <cmath>

#include <SFML/Graphics/Rect.hpp>

#include "Vector2.hpp"

namespace rat {
	class Rect {
	public:

		float x;
		float y;
		float width;
		float height;

		Rect() = default;

		Rect(const Rect&) = default;

		Rect& operator = (const Rect&) = default;

		Rect(float x, float y, float width, float height) :
			x(x), y(y), width(width), height(height) {}

		Rect(const Vector2& position, const Vector2& size) :
			x(position.x), y(position.y), width(size.x), height(size.y) {}

		template <typename T>
		Rect(const sf::Rect<T>& sfRect) :
			x(sfRect.x), y(sfRect.y), width(sfRect.width), height(sfRect.height) {}

		template <typename T>
		operator sf::Rect<T> () const {
			return sf::Rect<T>(x, y, width, height);
		}

		bool contains(const Vector2& point) const {
			return point.x >= x && point.y >= y && point.x <= x + width && point.y <= y + height;
		}

		bool contains(const Rect& rect) const {
			return rect.x >= x && rect.y >= y && rect.x + rect.width <= x + width && rect.y + rect.height <= y + height;
		}

		void setPosition(const Vector2& position) {
			x = position.x;
			y = position.y;
		}

		Vector2 getPosition() const {
			return { x, y };
		}

		void setSize(const Vector2& size) {
			width = size.x;
			height = size.y;
		}

		Vector2 getSize() const {
			return { width, height };
		}

		static Rect commonPart(const Rect& rect1, const Rect& rect2) const {
			float bx = std::max(rect1.x, rect2.x);
			float by = std::max(rect1.y, rect2.y);
			float ex = std::min(rect1.x + rect1.width, rect2.x + rect2.width);
			float ey = std::min(rect1.y + rect1.height, rect2.y + rect2.height);

			if (bx > ex || by > ey) return {};

			return { bx, by, ex-bx, ey-by };
		}
	};
}
