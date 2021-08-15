#ifndef _ISM_RECT_HPP_
#define _ISM_RECT_HPP_

#include <core/math/vector4.hpp>

namespace ism
{
	struct Rect
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename Vec4::value_type;
		using pointer					= typename Vec4::pointer;
		using const_pointer				= typename Vec4::const_pointer;
		using reference					= typename Vec4::reference;
		using const_reference			= typename Vec4::const_reference;
		using iterator					= typename Vec4::iterator;
		using const_iterator			= typename Vec4::const_iterator;
		using reverse_iterator			= typename Vec4::reverse_iterator;
		using const_reverse_iterator	= typename Vec4::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Rect() noexcept : m_data{} {}

		template <class X = float_t, class Y = float_t, class Z = float_t, class W = float_t
		> constexpr Rect(X x, Y y, Z z, W w) noexcept : m_data{ (float_t)x, (float_t)y, (float_t)z, (float_t)w } {}

		constexpr Rect(Vec2 const & pos, Vec2 const & size) : m_data{ pos[0], pos[1], size[0], size[1] } {}

		constexpr Rect(Vec4 const & value) : m_data{ value } {}

		constexpr Rect(Vec4 && value) noexcept : m_data{ std::move(value) } {}

		constexpr Rect(Rect const & other) : m_data{ other.m_data } {}

		constexpr Rect(Rect && other) noexcept : m_data{} { this->swap(std::move(other)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Rect & operator=(Rect const & other)
		{
			Rect temp{ other };
			this->swap(temp);
			return (*this);
		}

		constexpr Rect & operator=(Rect && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		constexpr void swap(Rect & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_data.swap(other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto data() noexcept -> pointer { return m_data.data(); }

		NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data.data(); }

		NODISCARD constexpr bool empty() const noexcept { return m_data.empty(); }

		NODISCARD constexpr auto max_size() const noexcept -> size_t { return m_data.max_size(); }

		NODISCARD constexpr auto size() const noexcept -> size_t { return m_data.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr operator pointer() noexcept { return m_data.operator pointer(); }

		NODISCARD constexpr operator const_pointer() const noexcept { return m_data.operator const_pointer(); }

		NODISCARD constexpr auto operator*() & noexcept -> reference { return m_data.operator*(); }

		NODISCARD constexpr auto operator*() const & noexcept -> const_reference { return m_data.operator*(); }

		NODISCARD constexpr auto operator*() && noexcept -> float_t && { return std::move(m_data.operator*()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto at(size_t const i) & noexcept -> reference { return m_data[i]; }

		NODISCARD constexpr auto at(size_t const i) const & noexcept -> const_reference { return m_data[i]; }

		NODISCARD constexpr auto at(size_t const i) && noexcept -> value_type && { return std::move(m_data[i]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto begin() noexcept -> iterator { return m_data.begin(); }

		NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }

		NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }

		NODISCARD constexpr auto end() noexcept -> iterator { return m_data.end(); }

		NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }

		NODISCARD constexpr auto cend() const noexcept -> const_iterator { return m_data.cend(); }

		NODISCARD constexpr auto rbegin() noexcept -> reverse_iterator { return m_data.rbegin(); }

		NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_data.rbegin(); }

		NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_data.crbegin(); }

		NODISCARD constexpr auto rend() noexcept -> reverse_iterator { return m_data.rend(); }

		NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return m_data.rend(); }

		NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return m_data.crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// define additional code
#ifdef ISM_RECT_EXTRA
		ISM_RECT_EXTRA
#endif // ISM_RECT_EXTRA

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Vec4 m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_RECT_HPP_
