#pragma once

#ifndef SUPER_COCO_VECTOR2_HPP
#define SUPER_COCO_VECOTR2_HPP

#include <string>
#include <SDL2/SDL_rect.h>

namespace Sce
{
	template<typename T>
	struct Vector2
	{
		T x, y;

		Vector2() = default;
		explicit Vector2(T V);
		Vector2(T X, T Y);
		Vector2(SDL_FPoint point);
		~Vector2() = default;
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) noexcept = default;

		Vector2& operator=(const Vector2&) = default;
		Vector2& operator=(Vector2&&) = default;

		Vector2 operator+(const Vector2& vec) const;
		Vector2 operator-(const Vector2& vec) const;
		Vector2 operator*(const Vector2& vec) const;
		Vector2 operator*(T scalar) const;
		Vector2 operator/(T scalar) const;

		Vector2& operator+=(const Vector2& vec);
		Vector2& operator-=(const Vector2& vec);
		Vector2& operator*=(const Vector2& vec);
		Vector2& operator*=(T scalar);
		Vector2& operator/=(T scalar);

		operator SDL_FPoint() const;

		std::string ToString() const;

		T SquareLength();
		T Length();
		Vector2& Normalize();
		static T Distance(const Vector2<T>& vec1, const Vector2<T>& vec2);
		static T SquareDistance(const Vector2<T>& vec1, const Vector2<T>& vec2);
		static T Dot(const Vector2<T>& vec1, const Vector2<T>& vec2);

	};

	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;

}

#include <SuperCoco/Vector2.inl>

#endif