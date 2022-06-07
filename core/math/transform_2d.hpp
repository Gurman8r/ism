#ifndef _ISM_TRANSFORM_2D_HPP_
#define _ISM_TRANSFORM_2D_HPP_

#include <core/math/color.hpp>
#include <core/math/rect.hpp>
#include <core/math/quat.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Transform;

	class Transform2D
	{
		Mat4 m_matrix;

	public:
		using self_type					= typename Transform2D;
		using storage_type				= typename decltype(m_matrix);
		using value_type				= typename storage_type::value_type;
		using pointer					= typename storage_type::pointer;
		using const_pointer				= typename storage_type::const_pointer;
		using reference					= typename storage_type::reference;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

	public:
		constexpr Transform2D() noexcept : m_matrix{} {}

		constexpr Transform2D(
			float_t m00, float_t m01, float_t m02, float_t m03,
			float_t m10, float_t m11, float_t m12, float_t m13,
			float_t m20, float_t m21, float_t m22, float_t m23,
			float_t m30, float_t m31, float_t m32, float_t m33
		) : m_matrix{ m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 } {}

		constexpr Transform2D(storage_type const & value) : m_matrix{ value } {}

		constexpr Transform2D(storage_type && value) noexcept : m_matrix{ std::move(value) } {}
		
		constexpr Transform2D(Transform2D const & other) : m_matrix{ other.m_matrix } {}

		constexpr self_type & operator=(self_type const & other) { self_type temp{ other }; return swap(temp); }

		constexpr self_type & operator=(self_type && other) noexcept { return swap(other); }

		constexpr self_type & swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_matrix, other.m_matrix);
			}
			return (*this);
		}

	public:
		// define additional code
#ifdef ISM_TRANSFORM2D_EXTRA
		ISM_TRANSFORM2D_EXTRA
#endif // ISM_TRANSFORM2D_EXTRA
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRANSFORM_2D_HPP_
