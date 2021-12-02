#ifndef _ISM_MATRIX_HPP_
#define _ISM_MATRIX_HPP_

#include <core/templates/array.hpp>

namespace ism
{
	template <class _T, size_t _Width, size_t _Height
	> struct NODISCARD Matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < _Width, "Matrix width negative or zero");

		static_assert(0 < _Height, "Matrix height negative or zero");

		static_assert(std::is_scalar_v<_T>, "Matrix only supports scalar types");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename _T;
		using self_type					= typename Matrix<value_type, _Width, _Height>;
		using storage_type				= typename Array<value_type, _Width * _Height>;
		using size_type					= typename storage_type::size_type;
		using difference_type			= typename storage_type::difference_type;
		using coord_type				= typename Array<size_type, 2>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using rvalue					= typename storage_type::rvalue;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_data.swap(other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto data() noexcept -> pointer { return m_data.data(); }

		NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data.data(); }

		NODISCARD constexpr bool empty() const noexcept { return false; }

		NODISCARD constexpr auto height() const noexcept -> size_t { return _Height; }

		NODISCARD constexpr auto max_size() const noexcept -> size_t { return m_data.max_size(); }

		NODISCARD constexpr auto size() const noexcept -> size_t { return m_data.size(); }

		NODISCARD constexpr auto width() const noexcept -> size_t { return _Width; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr operator storage_type & () & noexcept { return m_data; }

		NODISCARD constexpr operator storage_type const & () const & noexcept { return m_data; }

		NODISCARD constexpr operator storage_type && () && noexcept { return std::move(m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr operator pointer() noexcept { return m_data; }

		NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto operator*() & noexcept -> reference { return (*m_data); }

		NODISCARD constexpr auto operator*() const & noexcept -> const_reference { return (*m_data); }

		NODISCARD constexpr auto operator*() && noexcept -> rvalue { return std::move(*m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto at(size_t const i) & noexcept -> reference { return m_data.at(i); }
		
		NODISCARD constexpr auto at(size_t const i) const & noexcept -> const_reference { return m_data.at(i); }

		NODISCARD constexpr auto at(size_t const i) && noexcept -> rvalue { return std::move(m_data.at(i)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto at(size_t const x, size_t const y) & noexcept -> reference { return at(y * _Width + x); }

		NODISCARD constexpr auto at(size_t const x, size_t const y) const & noexcept -> const_reference { return at(y * _Width + x); }

		NODISCARD constexpr auto at(size_t const x, size_t const y) && noexcept -> rvalue { return std::move(at(y * _Width + x)); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto at(coord_type const & loc) & noexcept -> reference { return at(loc[0], loc[1]); }

		NODISCARD constexpr auto at(coord_type const & loc) const & noexcept -> const_reference { return at(loc[0], loc[1]); }

		NODISCARD constexpr auto at(coord_type const & loc) && noexcept -> rvalue { return std::move(at(loc[0], loc[1])); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto back() & noexcept -> reference { return m_data.back(); }

		NODISCARD constexpr auto back() const & noexcept -> const_reference { return m_data.back(); }

		NODISCARD constexpr auto back() && noexcept -> rvalue { return std::move(m_data.back()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr auto front() & noexcept -> reference { return m_data.front(); }

		NODISCARD constexpr auto front() const & noexcept -> const_reference { return m_data.front(); }

		NODISCARD constexpr auto front() && noexcept -> rvalue { return std::move(m_data.front()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		NODISCARD constexpr auto begin() noexcept -> iterator { return m_data.begin(); }

		NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }

		NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }

		NODISCARD constexpr auto cend() const noexcept -> const_iterator { return m_data.cend(); }

		NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_data.crbegin(); }

		NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return m_data.crend(); }
		
		NODISCARD constexpr auto end() noexcept -> iterator { return m_data.end(); }

		NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }

		NODISCARD constexpr auto rbegin() noexcept -> reverse_iterator { return m_data.rbegin(); }

		NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_data.rbegin(); }

		NODISCARD constexpr auto rend() noexcept -> reverse_iterator { return m_data.rend(); }

		NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return m_data.rend(); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr bool nonzero() const noexcept
		{
			for (auto const & e : m_data)
				if (e != (value_type)0)
					return true;
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto map_coord(size_t i) noexcept -> coord_type
		{
			return { i % _Width, i / _Height };
		}

		static constexpr auto map_coord(size_t x, size_t y) noexcept -> size_t
		{
			return y * _Width + x;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr self_type zero() noexcept
		{
			return self_type{};
		}

		static constexpr self_type fill(value_type value) noexcept
		{
			self_type temp{};
			for (auto & e : temp) { e = value; }
			return temp;
		}

		static constexpr self_type one() noexcept
		{
			return fill((value_type)1);
		}

		static constexpr self_type identity() noexcept
		{
			self_type temp{};
			for (size_t i = 0; i < (_Width * _Height); ++i)
			{
				if ((i / _Width) == (i % _Width))
				{
					temp[i] = (value_type)1;
				}
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U, size_t W, size_t H
		> NODISCARD constexpr operator Matrix<U, W, H>() const noexcept
		{
			using Other = Matrix<U, W, H>;
			if constexpr (std::is_same_v<Other, self_type>)
			{
				return (*this); // same type
			}
			else
			{
				Other temp{};
				if constexpr ((W == _Width) && (H == _Height))
				{
					// same dimensions
					for (size_t i = 0; i < (W * H); ++i)
					{
						temp[i] = static_cast<U>(this->at(i));
					}
				}
				else
				{
					// different dimensions
					for (size_t i = 0; i < (W * H); ++i)
					{
						if (size_t const x{ i % W }, y{ i / W }; (x < _Width && y < _Height))
						{
							temp[i] = static_cast<U>(this->at(x, y));
						}
					}
				}
				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// define additional code
#ifdef ISM_MATRIX_CLASS_EXTRA
		ISM_MATRIX_CLASS_EXTRA
#endif // ISM_MATRIX_CLASS_EXTRA

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// ALIASES
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix<T, N, N>
	template <class T, size_t N
	> ALIAS(tmatnxn) Matrix<T, N, N>;

	// Matrix<T, N, 1>
	template <class T, size_t N
	> ALIAS(tvector) Matrix<T, N, 1>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix<T, 2, 2>
	template <class T
	> ALIAS(tmat2) tmatnxn<T, 2>;
	ALIAS(mat2b) tmat2<byte>;
	ALIAS(mat2i) tmat2<int32_t>;
	ALIAS(mat2u) tmat2<uint32_t>;
	ALIAS(mat2f) tmat2<float32_t>;
	ALIAS(mat2d) tmat2<float64_t>;
	ALIAS(mat2s) tmat2<size_t>;
	ALIAS(mat2) mat2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix<T, 3, 3>
	template <class T
	> ALIAS(tmat3) tmatnxn<T, 3>;
	ALIAS(mat3b) tmat3<byte>;
	ALIAS(mat3i) tmat3<int32_t>;
	ALIAS(mat3u) tmat3<uint32_t>;
	ALIAS(mat3f) tmat3<float32_t>;
	ALIAS(mat3d) tmat3<float64_t>;
	ALIAS(mat3s) tmat3<size_t>;
	ALIAS(mat3) mat3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix<T, 4 ,4>
	template <class T
	> ALIAS(tmat4) tmatnxn<T, 4>;
	ALIAS(mat4b) tmat4<byte>;
	ALIAS(mat4i) tmat4<int32_t>;
	ALIAS(mat4u) tmat4<uint32_t>;
	ALIAS(mat4f) tmat4<float32_t>;
	ALIAS(mat4d) tmat4<float64_t>;
	ALIAS(mat4s) tmat4<size_t>;
	ALIAS(mat4) mat4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix<T, 2, 1>
	template <class T
	> ALIAS(tvec2) tvector<T, 2>;
	ALIAS(vec2b) tvec2<byte>;
	ALIAS(vec2i) tvec2<int32_t>;
	ALIAS(vec2u) tvec2<uint32_t>;
	ALIAS(vec2f) tvec2<float32_t>;
	ALIAS(vec2d) tvec2<float64_t>;
	ALIAS(vec2s) tvec2<size_t>;
	ALIAS(vec2) vec2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix<T, 3, 1>
	template <class T
	> ALIAS(tvec3) tvector<T, 3>;
	ALIAS(vec3b) tvec3<byte>;
	ALIAS(vec3i) tvec3<int32_t>;
	ALIAS(vec3u) tvec3<uint32_t>;
	ALIAS(vec3f) tvec3<float32_t>;
	ALIAS(vec3d) tvec3<float64_t>;
	ALIAS(vec3s) tvec3<size_t>;
	ALIAS(vec3) vec3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix<T, 4, 1>
	template <class T
	> ALIAS(tvec4) tvector<T, 4>;
	ALIAS(vec4b) tvec4<byte>;
	ALIAS(vec4i) tvec4<int32_t>;
	ALIAS(vec4u) tvec4<uint32_t>;
	ALIAS(vec4f) tvec4<float32_t>;
	ALIAS(vec4d) tvec4<float64_t>;
	ALIAS(vec4s) tvec4<size_t>;
	ALIAS(vec4) vec4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// MATRIX OPERATORS
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr bool operator==(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data == rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr bool operator!=(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data != rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr bool operator<(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data < rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr bool operator<=(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data <= rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr bool operator>(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data > rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr bool operator>=(Matrix<Tx, W, H> const & lhs, Matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data >= rhs.m_data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr auto operator+=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept -> auto &
	{
		for (auto & e : lhs)
		{
			e += static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr auto operator-=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept -> auto &
	{
		for (auto & e : lhs)
		{
			e -= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr auto operator*=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept -> auto &
	{
		for (auto & e : lhs)
		{
			e *= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H
	> constexpr auto operator/=(Matrix<Tx, W, H> & lhs, Ty const rhs) noexcept -> auto &
	{
		for (auto & e : lhs)
		{
			e /= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr auto operator+(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp += static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr auto operator-(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp -= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr auto operator*(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp *= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> NODISCARD constexpr auto operator/(Matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
	{
		auto temp{ lhs };
		return temp /= static_cast<Tx>(rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> NODISCARD constexpr auto operator-(Matrix<T, W, H> const & lhs) noexcept
	{
		return (lhs * static_cast<T>(-1));
	}

	template <class T, size_t W, size_t H
	> NODISCARD constexpr auto operator+(Matrix<T, W, H> const & lhs) noexcept
	{
		return -(-(lhs));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// VECTOR OPERATORS
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto & operator+=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] += static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto & operator-=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] -= static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto & operator*=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] *= static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto & operator/=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] /= static_cast<Tx>(rhs[i]);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto operator+(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp += rhs;
	}

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto operator-(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp -= rhs;
	}

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto operator*(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp *= rhs;
	}

	template <class Tx, class Ty, size_t N
	> NODISCARD constexpr auto operator/(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
	{
		auto temp{ lhs };
		return temp /= rhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// TUPLE INTERFACE
namespace std
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t I, class T, size_t W, size_t H
	> NODISCARD constexpr T & get(ism::Matrix<T, W, H> & value) noexcept
	{
		static_assert(I < W * H, "ism::Matrix index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t W, size_t H
	> NODISCARD constexpr T const & get(ism::Matrix<T, W, H> const & value) noexcept
	{
		static_assert(I < W * H, "ism::Matrix index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t W, size_t H
	> NODISCARD constexpr T && get(ism::Matrix<T, W, H> && value) noexcept
	{
		static_assert(I < W * H, "ism::Matrix index out of bounds");
		return std::move(value.at(I));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> NODISCARD constexpr T & get(ism::Matrix<T, W, H> & value) noexcept
	{
		static_assert(X * Y < W * H, "ism::Matrix index out of bounds");
		return value.at(X, Y);
	}

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> NODISCARD constexpr T const & get(ism::Matrix<T, W, H> const & value) noexcept
	{
		static_assert(X * Y < W * H, "ism::Matrix index out of bounds");
		return value.at(X, Y);
	}

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> NODISCARD constexpr T && get(ism::Matrix<T, W, H> && value) noexcept
	{
		static_assert(X * Y < W * H, "ism::Matrix index out of bounds");
		return std::move(value.at(X, Y));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MATRIX_HPP_
