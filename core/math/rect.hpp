#ifndef _ISM_RECT_HPP_
#define _ISM_RECT_HPP_

#include <core/math/vector4.hpp>

namespace ism
{
	template <class _T> struct Rect
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename _T;
		using self_type					= typename Rect;
		using coord_type				= typename TVector2<value_type>;
		using storage_type				= typename TVector4<value_type>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Rect() noexcept : m_data{} {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class X1, class Y1, class X2, class Y2
		> constexpr Rect(X1 l, Y1 t, X2 r, Y2 b)
			: m_data{ (value_type)l, (value_type)t, (value_type)r, (value_type)b }
		{
		}

		constexpr Rect(coord_type const & min, coord_type const & max)
			: m_data{ min[0], min[1], max[0], max[1] }
		{
		}

		constexpr Rect(storage_type const & value)
			: m_data{ value }
		{
		}

		constexpr Rect(self_type const & other)
			: m_data{ other.m_data }
		{
		}

		constexpr Rect(self_type && other) noexcept
			: m_data{}
		{
			this->swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			Rect temp{ other };
			this->swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_data.swap(other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator storage_type & () & noexcept { return m_data; }

		constexpr operator storage_type const & () const & noexcept { return m_data; }

		constexpr operator storage_type && () && noexcept { return std::move(m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator pointer() noexcept { return m_data; }

		constexpr operator const_pointer() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr coord_type & min() & noexcept { return *(coord_type *)&m_data[0]; }

		constexpr coord_type const & min() const & noexcept { return *(coord_type const *)&m_data[0]; }

		template <class Value = coord_type
		> coord_type & min(Value && value) & noexcept { return min() = FWD(value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr coord_type & max() & noexcept { return *(coord_type *)&m_data[2]; }

		constexpr coord_type const & max() const & noexcept { return *(coord_type const *)&m_data[2]; }

		template <class Value = coord_type
		> coord_type & max(Value && value) & noexcept { return max() = FWD(value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr value_type & at(size_t i) & noexcept { return m_data[i]; }

		constexpr value_type & left() & noexcept { return m_data[0]; }

		constexpr value_type & top() & noexcept { return m_data[1]; }

		constexpr value_type & right() & noexcept { return m_data[2]; }

		constexpr value_type & bottom() & noexcept { return m_data[3]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr value_type const & at(size_t i) const & noexcept { return m_data[i]; }

		constexpr value_type const & left() const & noexcept { return m_data[0]; }

		constexpr value_type const & top() const & noexcept { return m_data[1]; }

		constexpr value_type const & right() const & noexcept { return m_data[2]; }

		constexpr value_type const & bottom() const & noexcept { return m_data[3]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Value = value_type
		> constexpr value_type & set(size_t i, Value && value) & noexcept { m_data[i] = FWD(value); }

		template <class Value = value_type
		> constexpr value_type & left(Value && value) & noexcept { m_data[0] = FWD(value); }

		template <class Value = value_type
		> constexpr value_type & top(Value && value) & noexcept { m_data[1] = FWD(value); }

		template <class Value = value_type
		> constexpr value_type & right(Value && value) & noexcept { m_data[2] = FWD(value); }

		template <class Value = value_type
		> constexpr value_type & bottom(Value && value) & noexcept { m_data[3] = FWD(value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr value_type width() const noexcept { return m_data[2] - m_data[0]; }

		constexpr value_type height() const noexcept { return m_data[3] - m_data[1]; }

		constexpr coord_type size() const noexcept { return { m_data[2] - m_data[0], m_data[3] - m_data[1] }; }

		constexpr coord_type center() const noexcept { return (min() + max()) / 2; }

		constexpr coord_type position() const noexcept { return min(); }

		constexpr coord_type top_left() const noexcept { return min(); }

		constexpr coord_type top_right() const noexcept { return { m_data[2], m_data[1] }; }

		constexpr coord_type bottom_left() const noexcept { return { m_data[0], m_data[3] }; }

		constexpr coord_type bottom_right() const noexcept { return max(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr auto begin() noexcept -> iterator { return m_data.begin(); }

		constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }

		constexpr auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }

		constexpr auto cend() const noexcept -> const_iterator { return m_data.cend(); }

		constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_data.crbegin(); }

		constexpr auto crend() const noexcept -> const_reverse_iterator { return m_data.crend(); }

		constexpr auto end() noexcept -> iterator { return m_data.end(); }

		constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }

		constexpr auto rbegin() noexcept -> reverse_iterator { return m_data.rbegin(); }

		constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_data.rbegin(); }

		constexpr auto rend() noexcept -> reverse_iterator { return m_data.rend(); }

		constexpr auto rend() const noexcept -> const_reverse_iterator { return m_data.rend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	ALIAS(FloatRect)	Rect<float_t>;
	ALIAS(DoubleRect)	Rect<double_t>;
	ALIAS(IntRect)		Rect<int32_t>;
	ALIAS(UintRect)		Rect<uint32_t>;
	ALIAS(SizeRect)		Rect<size_t>;
}

#endif // !_ISM_RECT_HPP_
