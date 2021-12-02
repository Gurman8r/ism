#ifndef _ISM_TRANSFORM_HPP_
#define _ISM_TRANSFORM_HPP_

#include <core/math/vector4.hpp>

namespace ism
{
	struct Transform
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type				= typename Mat4f;
		using value_type				= typename storage_type::value_type;
		using pointer					= typename storage_type::pointer;
		using const_pointer				= typename storage_type::const_pointer;
		using reference					= typename storage_type::reference;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Transform() noexcept : m_data{} {}

		constexpr Transform(
			float_t m00, float_t m01, float_t m02, float_t m03,
			float_t m10, float_t m11, float_t m12, float_t m13,
			float_t m20, float_t m21, float_t m22, float_t m23,
			float_t m30, float_t m31, float_t m32, float_t m33
		) : m_data{
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		} {}

		constexpr Transform(storage_type const & value) : m_data{ value } {}
		
		constexpr Transform(storage_type && value) noexcept : m_data{ std::move(value) } {}

		constexpr Transform(Transform const & other) : m_data{ other.m_data } {}
		
		constexpr Transform(Transform && other) noexcept : m_data{} { this->swap(std::move(other)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Transform & operator=(Transform const & other)
		{
			Transform temp{ other };
			this->swap(temp);
			return (*this);
		}

		constexpr Transform & operator=(Transform && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		constexpr void swap(Transform & other) noexcept
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
#ifdef ISM_TRANSFORM_EXTRA
		ISM_TRANSFORM_EXTRA
#endif // ISM_TRANSFORM_EXTRA

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data;
	};
}

#endif // !_ISM_TRANSFORM_HPP_
