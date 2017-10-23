#pragma once

#include <cmath>

#include <SFML/System/Vector2.hpp>

namespace rat {
    class Vector2 {
    public:

        float x;
        float y;

    	Vector2() = default;

    	Vector2(const Vector2&) = default;

    	Vector2& operator = (const Vector2&) = default;

    	Vector2(float x, float y) :
            x(x), y(y) {}

        template<typename T>
        Vector2(const sf::Vector2<T>& sfVec) :
            x(sfVec.x), y(sfVec.y) {}

        template<typename T>
        operator sf::Vector2<T> () const {
            return sf::Vector2<T>(x, y);
        }

        float sqrMagnitude() const {
            return x * x + y * y;
        }

    	float magnitude() const {
    		return std::sqrt(sqrMagnitude());
    	}

    	float angle() const {
    		return std::atan2(y, x) * 57.2957795131f;
    	}

        void normalize() {
    		float mag = magnitude();
    		x /= mag;
    		y /= mag;
    	}

    	Vector2 normalized() const {
    		float mag = magnitude();
    		return { x / mag, y / mag };
    	}

        void scale(const Vector2& scale) {
            x *= scale.x;
            y *= scale.y;
        }

    	Vector2 scaled(const Vector2& scale) const {
    		return { x * scale.x, y * scale.y };
    	}

    	void floor() {
            x = std::floor(x);
            y = std::floor(y);
    	}

    	Vector2 floored() const {
            return { std::floor(x), std::floor(y) };
    	}

        void round() {
            x = std::round(x);
            y = std::round(y);
        }

    	Vector2 rounded() const {
            return { std::round(x), std::round(y) };
    	}

        void ceil() {
            x = std::ceil(x);
            y = std::ceil(y);
        }

    	Vector2 ceiled() const {
            return { std::ceil(x), std::ceil(y) };
    	}

    	Vector2 operator - () const {
            return { -x, -y };
        }

    	Vector2 operator + (const Vector2& vec) const {
            return { x + vec.x,	y + vec.y };
        }

    	Vector2 operator - (const Vector2& vec) const {
            return { x - vec.x,	y - vec.y };
        }

    	Vector2 operator * (const Vector2& vec) const {
            return { x * vec.x,	y * vec.y };
        }

    	Vector2 operator / (const Vector2& vec) const {
            return { x / vec.x,	y / vec.y };
        }

    	Vector2 operator * (float val) const {
            return { x * val, y * val };
        }

    	Vector2 operator / (float val) const {
            return { x / val, y / val };
        }

    	Vector2& operator += (const Vector2& vec) {
            x += vec.x;
            y += vec.y;
            return *this;
        }

    	Vector2& operator -= (const Vector2& vec) {
            x -= vec.x;
            y -= vec.y;
            return *this;
        }

    	Vector2& operator *= (const Vector2& vec) {
            x *= vec.x;
            y *= vec.y;
            return *this;
        }

    	Vector2& operator /= (const Vector2& vec) {
            x /= vec.x;
            y /= vec.y;
            return *this;
        }

    	Vector2& operator *= (float val) {
            x *= val;
            y *= val;
            return *this;
        }

    	Vector2& operator /= (float val) {
            x /= val;
            y /= val;
            return *this;
        }

        bool operator == (const Vector2& vec) const {
            return std::abs(x - vec.x) < 1e-5f && std::abs(y - vec.y) < 1e-5f;
        }

        bool operator != (const Vector2& vec) const {
            return !operator==(vec);
        }

        bool operator < (const Vector2& vec) const {
            return x - vec.x < -1e-5 && y - vec.y < -1e-5;
        }

    	bool operator > (const Vector2& vec) const {
            return x - vec.x > 1e-5 && y - vec.y > 1e-5;
        }

        bool operator <= (const Vector2& vec) const {
            return !operator>(vec);
        }

        bool operator >= (const Vector2& vec) const {
            return !operator<(vec);
        }

        static Vector2 min(const Vector2& vec1, const Vector2& vec2) {
            return vec2 < vec1 ? vec2 : vec1;
        }

        static Vector2 max(const Vector2& vec1, const Vector2& vec2) {
            return vec1 < vec2 ? vec2 : vec1;
        }

        static float distance(const Vector2& vec1, const Vector2& vec2) {
            return (vec1 - vec2).magnitude();
        }

        static float dot(const Vector2& vec1, const Vector2& vec2) {
            return vec1.x * vec2.x + vec1.y * vec2.y;
        }

        static float angle(const Vector2& vec1, const Vector2& vec2) {
            float mag = std::sqrt(vec1.sqrMagnitude() * vec2.sqrMagnitude());
            return std::acos(dot(vec1, vec2) / mag) * 57.2957795131f;
        }
    };
}
