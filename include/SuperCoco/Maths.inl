#include <cmath>

namespace Sce
{
	template<typename T, typename U>
	inline T Maths::Pow(T value, U exponent)
	{
		return exponent >= 0.f 
			? static_cast<T>(pow(value, exponent))
			: 1.0 / Maths::Pow(value, -exponent);
	}

	template<typename T>
	inline T Maths::Clamp(T value, T min, T max)
	{
		return value > max ? max : min < value ? value : min;
	}

	template<typename T, typename U>
	inline T Maths::ExpDecay(T a, T b, T decay, U dt)
	{
		return b + ( a - b) * exp(-decay * dt);
	}

	template<typename T>
	inline T Maths::EaseInSine(T value)
	{
		return 1 - cos((value * PI) / 2);
	}

	template<typename T>
	inline T Maths::EaseOutSine(T value)
	{
		return sin((value * PI) / 2);
	}

	template<typename T>
	inline T Maths::EaseInOutSine(T value)
	{
		return -(cos(PI * value) - 1) / 2;
	}

	template<typename T>
	inline T Maths::EaseInQuad(T value)
	{
		return value * value;
	}

	template<typename T>
	inline T Maths::EaseOutQuad(T value)
	{
		return 1 - (1 - value) * (1 - value);
	}

	template<typename T>
	inline T Maths::EaseInOutQuad(T value)
	{
		return value < 0.5 ? 2 * value * value : 1 - Maths::Pow(-2 * value + 2, 2) / 2;
	}

	template<typename T>
	inline T Maths::EaseInCubic(T value)
	{
		return value * value * value;
	}

	template<typename T>
	inline T Maths::EaseOutCubic(T value)
	{
		return 1 - Maths::Pow(1 - value, 3);
	}

	template<typename T>
	inline T Maths::EaseInOutCubic(T value)
	{
		return value < 0.5 ? 4 * value * value * value : 1 - Maths::Pow(-2 * value + 2, 3) / 2;
	}

	template<typename T>
	inline T Maths::EaseInQuart(T value)
	{
		return value * value * value * value;
	}

	template<typename T>
	inline T Maths::EaseOutQuart(T value)
	{
		return 1 - Maths::Pow(1 - value, 4);
	}

	template<typename T>
	inline T Maths::EaseInOutQuart(T value)
	{
		return value < 0.5 ? 8 * value * value * value * value : 1 - Maths::Pow(-2 * value + 2, 4) / 2;
	}

	template<typename T>
	inline T Maths::EaseInQuint(T value)
	{
		return value * value * value * value * value;
	}

	template<typename T>
	inline T Maths::EaseOutQuint(T value)
	{
		return 1 - Maths::Pow(1 - value, 5);
	}

	template<typename T>
	inline T Maths::EaseInOutQuint(T value)
	{
		return value < 0.5 ? 16 * value * value * value * value * value : 1 - Maths::Pow(-2 * value + 2, 5) / 2;
	}

	template<typename T>
	inline T Maths::EaseInExpo(T value)
	{
		return value == 0 ? 0 : Maths::Pow(2, 10 * value - 10);
	}

	template<typename T>
	inline T Maths::EaseOutExpo(T value)
	{
		return value == 1 ? 1 : 1 - Maths::Pow(2, -10 * value);
	}

	template<typename T>
	inline T Maths::EaseInOutExpo(T value)
	{
		return value == 0
			? 0
			: value == 1
			? 1
			: value < 0.5 ? Maths::Pow(2, 20 * value - 10) / 2
			: (2 - Maths::Pow(2, -20 * value + 10)) / 2;
	}

	template<typename T>
	inline T Maths::EaseInCirc(T value)
	{
		return 1 - sqrt(1 - Maths::Pow(value, 2));
	}

	template<typename T>
	inline T Maths::EaseOutCirc(T value)
	{
		return sqrt(1 - Maths::Pow(value - 1, 2));
	}

	template<typename T>
	inline T Maths::EaseInOutCirc(T value)
	{
		return value < 0.5
			? (1- sqrt(1 - Maths::Pow(2 * value, 2))) / 2
			: (sqrt(1 - Maths::Pow(-2 * value * 2, 2)) + 1) / 2;
	}

	template<typename T>
	inline T Maths::EaseInBack(T value)
	{
		return c3 * value * value * value - c1 * value * value;
	}

	template<typename T>
	inline T Maths::EaseOutBack(T value)
	{
		return 1.f + (1.70158f + 1.f) * Maths::Pow(value - 1.f, 3.f) + 1.70158f * Maths::Pow(value - 1.f, 2.f);
	}

	template<typename T>
	inline T Maths::EaseInOutBack(T value)
	{
		return value < 0.5
			? (Maths::Pow(2 * value, 2) * ((c2 + 1) * 2 * value - c2)) / 2
			: (Maths::Pow(2 * value - 2, 2) * ((c2 + 1) * (value * 2 - 2) + c2) + 2) / 2;
	}

	template<typename T>
	inline T Maths::EaseInElastic(T value)
	{
		return value == 0
			? 0
			: value == 1
			? 1
			: -Maths::Pow(2, 10 * value - 10) * sin((value * 10 - 10.75) * c4);
	}

	template<typename T>
	inline T Maths::EaseOutElastic(T value)
	{
		return value == 0
			? 0
			: value == 1
			? 1
			: Maths::Pow(2, -10 * value) * sin((value * 10 - 0.75) * c4) + 1;
	}

	template<typename T>
	inline T Maths::EaseInOutElastic(T value)
	{
		return value == 0
			? 0
			: value == 1
			? 1
			: value < 0.5
			? -(Maths::Pow(2, 20 * value - 10) * sin((20 * value - 11.125) * c5)) / 2
			: (Maths::Pow(2, -20 * value + 10) * sin((20 * value - 11.125) * c5)) / 2 + 1;
	}

	template<typename T>
	inline T Maths::EaseInBounce(T value)
	{
		return 1 - Maths::EaseOutBounce(1 - value);
	}

	template<typename T>
	inline T Maths::EaseOutBounce(T value)
	{
		if (value < 1 / d1)
		{
			return n1 * value * value;
		}
		else if (value < 2 / d1)
		{
			return n1 * (value -= 1.5 / d1) * value + 0.75;
		}
		else if (value < 2.5 / d1)
		{
			return n1 * (value -= 2.25 / d1) * value + 0.9375;
		}
		else
		{
			return n1 * (value -= 2.625 / d1) * value + 0.984375;
		};
	}

	template<typename T>
	inline T Maths::EaseInOutBounce(T value)
	{
		return value < 0.5
			? (1 - Maths::EaseOutBounce(1 - 2 * value)) / 2
			: (1 + Maths::EaseOutBounce(2 * value - 1)) / 2;
	}

	template<typename T>
	inline T Maths::EaseParabolic2(T value)
	{
		return -4 * Maths::Pow(value - 0.5, 2.0) + 1;
	}

	template<typename T>
	inline T Maths::EaseParabolic4(T value)
	{
		return -16 * Maths::Pow(value - 0.5, 4.0) + 1;
	}

	template<typename T>
	inline T Maths::EaseParabolic6(T value)
	{
		return -64 * Maths::Pow(value - 0.5, 6.0) + 1;
	}
}
