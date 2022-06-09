#ifndef _ISM_TRIG_HPP_
#define _ISM_TRIG_HPP_

#include <core/math/matrix.hpp>

#include <gcem.hpp>

#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/quaternion.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Math final
	{
		template <class T = float32_t, class = std::enable_if_t<std::is_floating_point_v<T>>
		> static constexpr T pi{ static_cast<T>(3.14159265358979323846) };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = float_t> constexpr auto deg2rad(T const degrees) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return degrees * static_cast<T>(0.01745329251994329576923690768489);
		}
		else
		{
			return deg2rad(static_cast<float_t>(degrees));
		}
	}

	template <class T = float_t> constexpr auto rad2deg(T const radians) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return radians * static_cast<T>(57.295779513082320876798154814105);
		}
		else
		{
			return rad2deg(static_cast<float_t>(radians));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = float_t
	> constexpr auto cross(TVector3<T> const & a, TVector3<T> const & b) noexcept -> TVector3<T>
	{
		static_assert(std::is_floating_point_v<T>);

		return { a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] };
	}

	template <class T = float_t
	> constexpr auto dot(TVector3<T> const & a, TVector3<T> const & b) noexcept -> T
	{
		static_assert(std::is_floating_point_v<T>);

		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	template <class T = float_t
	> constexpr auto normalize(TVector3<T> const & a) noexcept -> TVector3<T>
	{
		static_assert(std::is_floating_point_v<T>);

		float_t const il{ 1.f / (gcem::sqrt(dot(a, a)) + FLT_EPSILON) };

		return { a[0] * il, a[1] * il, a[2] * il };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRIG_HPP_
