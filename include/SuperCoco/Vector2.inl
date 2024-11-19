#include <cmath>
#include "Vector2.hpp"

namespace Sce
{
	template<typename T>
	Vector2<T>::Vector2(T V) :
	x(V),
	y(V)
	{

	}

	template<typename T>
	Vector2<T>::Vector2(T X, T Y) :
	x(X), y(Y)
	{
	}

	template<typename T>
	Vector2<T>::Vector2(SDL_FPoint point) :
	x(point.x),
	y(point.y)
	{
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2& vec) const
	{
		return Vector2{ x + vec.x, y + vec.y };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2& vec) const
	{
		return Vector2{ x - vec.x, y - vec.y };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(const Vector2& vec) const
	{
		return Vector2{ x * vec.x, y * vec.y };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(T scalar) const
	{
		return Vector2{ x * scalar, y * scalar };
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator/(T scalar) const
	{
		return Vector2{ x / scalar, y / scalar };
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator-=(const Vector2& vec)
	{
		x -= vec.y;
		y -= vec.y;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator*=(const Vector2& vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator/=(T scalar)
	{
		x /= scalar.x;
		y /= scalar.y;

		return *this;
	}

	template<typename T>
	Vector2<T>::operator SDL_FPoint() const
	{
		return SDL_FPoint{ x, y };
	}

	template<typename T>
	std::string Vector2<T>::ToString() const
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	template<typename T>
	T Vector2<T>::SquareLength()
	{
		return (x * x) + (y * y);
	}

	template<typename T>
	T Vector2<T>::Length()
	{
		return std::sqrt(static_cast<double>(SquareLength()));
	}

	template<typename T>
	Vector2<T>& Vector2<T>::Normalize()
	{
		if (x == 0 && y == 0)
			return *this;

		T len = Length();
		x /= len;
		y /= len;

		return *this;
	}

	template<typename T>
	T Vector2<T>::Distance(const Vector2<T>& vec1, const Vector2<T>& vec2)
	{
		return (vec2 - vec1).Length();
	}

	template<typename T>
	inline T Vector2<T>::SquareDistance(const Vector2<T>& vec1, const Vector2<T>& vec2)
	{
		return (vec2 - vec1).SquareLength();
	}

	template<typename T>
	T Vector2<T>::Dot(const Vector2<T>& vec1, const Vector2<T>& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
}