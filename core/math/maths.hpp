#ifndef _ISM_TRIG_HPP_
#define _ISM_TRIG_HPP_

#include <core/math/matrix.hpp>

#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/quaternion.hpp>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = f32, class = std::enable_if_t<std::is_floating_point_v<T>>
	> constexpr T pi{ static_cast<T>(3.14159265358979323846) };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = f32> constexpr auto deg2rad(T const degrees) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return degrees * static_cast<T>(0.01745329251994329576923690768489);
		}
		else
		{
			return deg2rad(static_cast<f32>(degrees));
		}
	}

	template <class T = f32> constexpr auto rad2deg(T const radians) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return radians * static_cast<T>(57.295779513082320876798154814105);
		}
		else
		{
			return rad2deg(static_cast<f32>(radians));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRIG_HPP_
