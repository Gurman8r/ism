#ifndef _ISM_MATH_HPP_
#define _ISM_MATH_HPP_

#include <core/templates/type_traits.hpp>

#include <gcem.hpp>

//#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/quaternion.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = f32, class = std::enable_if_t<std::is_floating_point_v<T>>
	> constexpr T pi_v{ static_cast<T>(3.14159265358979323846) };

	template <class T = f32
	> constexpr auto radians(T const angle_in_degrees) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return angle_in_degrees * static_cast<T>(0.01745329251994329576923690768489);
		}
		else
		{
			return radians(static_cast<f32>(angle_in_degrees));
		}
	}

	template <class T = f32
	> constexpr auto degrees(T const angle_in_radians) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return angle_in_radians * static_cast<T>(57.295779513082320876798154814105);
		}
		else
		{
			return degrees(static_cast<f32>(angle_in_radians));
		}
	}

	template <class T> NODISCARD auto asin(T const a) noexcept { return std::asin(a); }

	template <class T> NODISCARD auto acos(T const a) noexcept { return std::acos(a); }

	template <class T> NODISCARD auto atan(T const a) noexcept { return std::atan(a); }

	template <class T> NODISCARD auto atan2(T const a, T const b) noexcept { return std::atan2(a, b); }

	template <class T> NODISCARD auto sin(T const a) noexcept { return std::sin(a); }

	template <class T> NODISCARD auto cos(T const a) noexcept { return std::cos(a); }

	template <class T> NODISCARD auto tan(T const a) noexcept { return std::tan(a); }

	template <class T> NODISCARD auto sqrt(T const a) noexcept { return std::sqrt(a); }

	template <class T> NODISCARD auto inversesqrt(T const a) noexcept { return static_cast<decltype(sqrt(a))>(1) / sqrt(a); }

	template <class T> NODISCARD auto abs(T const a) noexcept { return std::abs(a); }

	template <class T> NODISCARD auto pow(T const a, T const b) noexcept { return std::pow(a, b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MATH_HPP_
