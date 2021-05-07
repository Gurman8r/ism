#ifndef _ISM_ARRAY_HPP_
#define _ISM_ARRAY_HPP_

#include <core/templates/utility.hpp>

// FIXED SIZE ARRAY
namespace ism
{
	template <class _Ty, size_t _N
	> class NODISCARD Array
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename _Ty;
		using self_type					= typename Array<value_type, _N>;
		using storage_type				= typename value_type[_N];
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;
		using pointer					= typename value_type *;
		using const_pointer				= typename value_type const *;
		using reference					= typename value_type &;
		using const_reference			= typename value_type const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				for (size_t i = 0; i < _N; ++i)
				{
					ism::util::swap(m_data[i], other.m_data[i]);
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto data() noexcept -> pointer { return m_data; }
		
		NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }
		
		NODISCARD constexpr bool empty() const noexcept { return false; }

		NODISCARD constexpr auto max_size() const noexcept -> size_type { return _N; }

		NODISCARD constexpr auto size() const noexcept -> size_type { return _N; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr operator pointer() noexcept { return m_data; }

		NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto operator*() & noexcept -> reference { return (*m_data); }

		NODISCARD constexpr auto operator*() const & noexcept -> const_reference { return (*m_data); }

		NODISCARD constexpr auto operator*() && noexcept -> value_type && { return std::move(*m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto at(size_type const i) & noexcept -> reference { return m_data[i]; }

		NODISCARD constexpr auto at(size_type const i) const & noexcept -> const_reference { return m_data[i]; }

		NODISCARD constexpr auto at(size_type const i) && noexcept -> value_type && { return std::move(m_data[i]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto back() & noexcept -> reference { return m_data[_N - 1]; }
		
		NODISCARD constexpr auto back() const & noexcept -> const_reference { return m_data[_N - 1]; }

		NODISCARD constexpr auto back() && noexcept -> value_type && { return std::move(m_data[_N - 1]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto front() & noexcept -> reference { return m_data[0]; }

		NODISCARD constexpr auto front() const & noexcept -> const_reference { return m_data[0]; }

		NODISCARD constexpr auto front() && noexcept -> value_type && { return std::move(m_data[0]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto begin() noexcept -> iterator { return m_data; }

		NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data; }

		NODISCARD constexpr auto end() noexcept -> iterator { return m_data + _N; }

		NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data + _N; }

		NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }

		NODISCARD constexpr auto cend() const noexcept -> const_iterator { return end(); }

		NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }

		NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return rend(); }

		NODISCARD constexpr auto rbegin() noexcept -> reverse_iterator { return std::make_reverse_iterator(end()); }

		NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cend()); }

		NODISCARD constexpr auto rend() noexcept -> reverse_iterator { return std::make_reverse_iterator(begin()); }

		NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cbegin()); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// ZERO SIZE ARRAY
namespace ism
{
	template <class _Ty
	> class NODISCARD Array<_Ty, 0>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type				= typename _Ty;
		using self_type					= typename Array<value_type, 0>;
		using storage_type				= typename value_type[1];
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;
		using pointer					= typename value_type *;
		using const_pointer				= typename value_type const *;
		using reference					= typename value_type &;
		using const_reference			= typename value_type const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		storage_type m_data; // aggregate initializer
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto data() noexcept -> pointer { return &m_data[0]; }

		NODISCARD constexpr auto data() const noexcept -> const_pointer { return &m_data[0]; }

		NODISCARD constexpr bool empty() const noexcept { return true; }

		NODISCARD constexpr auto max_size() const noexcept -> size_type { return 0; }

		NODISCARD constexpr auto size() const noexcept -> size_type { return 0; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr operator pointer() noexcept { return m_data; }

		NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto operator*() & noexcept -> reference { return (*m_data); }

		NODISCARD constexpr auto operator*() const & noexcept -> const_reference { return (*m_data); }

		NODISCARD constexpr auto operator*() && noexcept -> value_type && { return std::move(*m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto at(size_type) & noexcept -> reference { return m_data[0]; }

		NODISCARD constexpr auto at(size_type) const & noexcept -> const_reference { return m_data[0]; }

		NODISCARD constexpr auto at(size_type) && noexcept -> value_type && { return std::move(m_data[0]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto back() & noexcept -> reference { return m_data[0]; }

		NODISCARD constexpr auto back() const & noexcept -> const_reference { return m_data[0]; }

		NODISCARD constexpr auto back() && noexcept -> value_type && { return std::move(m_data[0]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto front() & noexcept -> reference { return m_data[0]; }

		NODISCARD constexpr auto front() const & noexcept -> const_reference { return m_data[0]; }

		NODISCARD constexpr auto front() && noexcept -> value_type && { return std::move(m_data[0]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto begin() noexcept -> iterator { return &m_data[0]; }

		NODISCARD constexpr auto begin() const noexcept -> const_iterator { return &m_data[0]; }

		NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return &m_data[0]; }

		NODISCARD constexpr auto cend() const noexcept -> const_iterator { return &m_data[0]; }

		NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return &m_data[0]; }

		NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		
		NODISCARD constexpr auto end() noexcept -> iterator { return &m_data[0]; }

		NODISCARD constexpr auto end() const noexcept -> const_iterator { return &m_data[0]; }

		NODISCARD constexpr auto rbegin() noexcept -> reverse_iterator { return &m_data[0]; }

		NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return &m_data[0]; }

		NODISCARD constexpr auto rend() noexcept -> reverse_iterator { return &m_data[0]; }

		NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// ARRAY OPERATORS
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> constexpr bool operator==(Array<Tx, N> const & lhs, Array<Ty, N> const & rhs)
	{
		return util::range_equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator!=(Array<Tx, N> const & lhs, Array<Ty, N> const & rhs)
	{
		return !(lhs == rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<(Array<Tx, N> const & lhs, Array<Ty, N> const & rhs)
	{
		return util::range_less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<=(Array<Tx, N> const & lhs, Array<Ty, N> const & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>(Array<Tx, N> const & lhs, Array<Ty, N> const & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>=(Array<Tx, N> const & lhs, Array<Ty, N> const & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// TUPLE INTERFACE
namespace std
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t I, class T, size_t N
	> NODISCARD constexpr T & get(ism::Array<T, N> & value) noexcept
	{
		static_assert(I < N, "Array index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t N
	> NODISCARD constexpr T const & get(ism::Array<T, N> const & value) noexcept
	{
		static_assert(I < N, "Array index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t N
	> NODISCARD constexpr T && get(ism::Array<T, N> && value) noexcept
	{
		static_assert(I < N, "Array index out of bounds");
		return std::move(value.at(I));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// MAKE ARRAY
namespace ism
{
	template <class D, class... Args
	> using array_return_type = Array<typename mpl::return_type_t<D, Args...>, sizeof...(Args)>;

	template <class D = void, class... Args
	> NODISCARD constexpr array_return_type<D, Args...> make_array(Args && ... args)
	{
		return { FWD(args)... };
	}
}

#endif // !_ISM_ARRAY_HPP_
