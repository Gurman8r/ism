#ifndef _ISM_TRANSFORM_HPP_
#define _ISM_TRANSFORM_HPP_

#include <core/math/rect.hpp>
#include <core/math/quat.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Transform2D;

	class Transform
	{
		Mat4 m_matrix;

	public:
		using self_type					= typename Transform;
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
		constexpr Transform() noexcept : m_matrix{} {}

		constexpr Transform(
			value_type m00, value_type m01, value_type m02, value_type m03,
			value_type m10, value_type m11, value_type m12, value_type m13,
			value_type m20, value_type m21, value_type m22, value_type m23,
			value_type m30, value_type m31, value_type m32, value_type m33
		) : m_matrix{ m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 } {}

		constexpr Transform(storage_type const & value) : m_matrix{ value } {}

		constexpr Transform(storage_type && value) noexcept : m_matrix{ std::move(value) } {}

		constexpr Transform(self_type const & other) : m_matrix{ other.m_matrix } {}
		
		constexpr self_type & operator=(self_type const & other) { self_type temp{ other }; return swap(temp); }

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
#ifdef ISM_TRANSFORM_EXTRA
		ISM_TRANSFORM_EXTRA
#endif // ISM_TRANSFORM_EXTRA
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TRANSFORM_HPP_
