#ifndef SUPER_COCO_MATHS_HPP
#define SUPER_COCO_MATH_HPP

#pragma once

	#define PI 3.1415926535f

	#define Deg2Rad 0.01745329251f
	#define Rad2Deg 57.2957795131f

	#define c1 1.70158f
	#define c2 c1 * 1.525f
	#define c3 c1 + 1.f
	#define c4 (2.f * PI) / 3.f
	#define c5 (2.f * PI) / 4.5f
	#define n1 7.5625
	#define d1 2.75

namespace Sce
{
	struct Maths
	{
		template<typename T, typename U>
		static T Pow(T value, U exponent);

		template<typename T>
		static T Clamp(T value, T min, T max);

		template<typename T, typename U>
		static T ExpDecay(T a, T b, T decay, U dt);

		template<typename T>
		static T Lerp(T a, T b, T dt);

		template<typename T>
		static T EaseInSine(T value);

		template<typename T>
		static T EaseOutSine(T value);

		template<typename T>
		static T EaseInOutSine(T value);

		template<typename T>
		static T EaseInQuad(T value);

		template<typename T>
		static T EaseOutQuad(T value);

		template<typename T>
		static T EaseInOutQuad(T value);

		template<typename T>
		static T EaseInCubic(T value);

		template<typename T>
		static T EaseOutCubic(T value);

		template<typename T>
		static T EaseInOutCubic(T value);

		template<typename T>
		static T EaseInQuart(T value);

		template<typename T>
		static T EaseOutQuart(T value);

		template<typename T>
		static T EaseInOutQuart(T value);

		template<typename T>
		static T EaseInQuint(T value);

		template<typename T>
		static T EaseOutQuint(T value);

		template<typename T>
		static T EaseInOutQuint(T value);

		template<typename T>
		static T EaseInExpo(T value);

		template<typename T>
		static T EaseOutExpo(T value);

		template<typename T>
		static T EaseInOutExpo(T value);

		template<typename T>
		static T EaseInCirc(T value);

		template<typename T>
		static T EaseOutCirc(T value);

		template<typename T>
		static T EaseInOutCirc(T value);

		template<typename T>
		static T EaseInBack(T value);

		template<typename T>
		static T EaseOutBack(T value);

		template<typename T>
		static T EaseInOutBack(T value);

		template<typename T>
		static T EaseInElastic(T value);

		template<typename T>
		static T EaseOutElastic(T value);

		template<typename T>
		static T EaseInOutElastic(T value);

		template<typename T>
		static T EaseInBounce(T value);

		template<typename T>
		static T EaseOutBounce(T value);

		template<typename T>
		static T EaseInOutBounce(T value);

		template<typename T>
		static T EaseParabolic2(T value);

		template<typename T>
		static T EaseParabolic4(T value);

		template<typename T>
		static T EaseParabolic6(T value);
	};

}

#include <SuperCoco/Maths.inl>

#endif