#ifndef _ISM_MATRIX_HPP_
#define _ISM_MATRIX_HPP_

#include <core/templates/array.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix
	template <class _Type, size_t _Width, size_t _Height
	> struct Matrix;

	// Matrix<T, N, N>
	template <class T, size_t N
	> ALIAS(TMatN) Matrix<T, N, N>;

	// Matrix<T, 2, 2>
	template <class T
	> ALIAS(TMat2) TMatN<T, 2>;

	// Matrix<T, 3, 3>
	template <class T
	> ALIAS(TMat3) TMatN<T, 3>;

	// Matrix<T, 4 ,4>
	template <class T
	> ALIAS(TMat4) TMatN<T, 4>;

	// Matrix<T, N, 1>
	template <class T, size_t N
	> ALIAS(TVecN) Matrix<T, N, 1>;

	// Matrix<T, 2, 1>
	template <class T
	> ALIAS(TVec2) TVecN<T, 2>;

	// Matrix<T, 3, 1>
	template <class T
	> ALIAS(TVec3) TVecN<T, 3>;

	// Matrix<T, 4, 1>
	template <class T
	> ALIAS(TVec4) TVecN<T, 4>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat2
	ALIAS(Mat2b) TMat2<u8>;
	ALIAS(Mat2i) TMat2<i32>;
	ALIAS(Mat2u) TMat2<u32>;
	ALIAS(Mat2f) TMat2<f32>;
	ALIAS(Mat2d) TMat2<f64>;
	ALIAS(Mat2s) TMat2<size_t>;
	ALIAS(Mat2) Mat2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat3
	ALIAS(Mat3b) TMat3<u8>;
	ALIAS(Mat3i) TMat3<i32>;
	ALIAS(Mat3u) TMat3<u32>;
	ALIAS(Mat3f) TMat3<f32>;
	ALIAS(Mat3d) TMat3<f64>;
	ALIAS(Mat3s) TMat3<size_t>;
	ALIAS(Mat3) Mat3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat4
	ALIAS(Mat4b) TMat4<u8>;
	ALIAS(Mat4i) TMat4<i32>;
	ALIAS(Mat4u) TMat4<u32>;
	ALIAS(Mat4f) TMat4<f32>;
	ALIAS(Mat4d) TMat4<f64>;
	ALIAS(Mat4s) TMat4<size_t>;
	ALIAS(Mat4) Mat4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec2
	ALIAS(Vec2b) TVec2<u8>;
	ALIAS(Vec2i) TVec2<i32>;
	ALIAS(Vec2u) TVec2<u32>;
	ALIAS(Vec2f) TVec2<f32>;
	ALIAS(Vec2d) TVec2<f64>;
	ALIAS(Vec2s) TVec2<size_t>;
	ALIAS(Vec2) Vec2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec3
	ALIAS(Vec3b) TVec3<u8>;
	ALIAS(Vec3i) TVec3<i32>;
	ALIAS(Vec3u) TVec3<u32>;
	ALIAS(Vec3f) TVec3<f32>;
	ALIAS(Vec3d) TVec3<f64>;
	ALIAS(Vec3s) TVec3<size_t>;
	ALIAS(Vec3) Vec3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec4
	ALIAS(Vec4b) TVec4<u8>;
	ALIAS(Vec4i) TVec4<i32>;
	ALIAS(Vec4u) TVec4<u32>;
	ALIAS(Vec4f) TVec4<f32>;
	ALIAS(Vec4d) TVec4<f64>;
	ALIAS(Vec4s) TVec4<size_t>;
	ALIAS(Vec4) Vec4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Matrix
	template <class _Type, size_t _Width, size_t _Height
	> struct Matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < _Width, "matrix width negative or zero");
		static_assert(0 < _Height, "matrix height negative or zero");
		static_assert(mpl::is_number_v<_Type>, "unsupported matrix type");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename _Type;
		using self_type					= typename Matrix<value_type, _Width, _Height>;
		using storage_type				= typename Array<value_type, _Width * _Height>;
		using size_type					= typename storage_type::size_type;
		using difference_type			= typename storage_type::difference_type;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & swap(self_type & other) noexcept { return util::swap(m_data, other.m_data), (*this); }

		NODISCARD static constexpr bool empty() noexcept { return false; }
		NODISCARD static constexpr auto width() noexcept -> size_t { return _Width; }
		NODISCARD static constexpr auto height() noexcept -> size_t { return _Height; }
		NODISCARD static constexpr auto capacity() noexcept -> size_t { return _Width * _Height; }
		NODISCARD static constexpr auto size() noexcept -> size_t { return _Width * _Height; }
		NODISCARD static constexpr auto max_size() noexcept -> size_t { return _Width * _Height; }
		
		NODISCARD constexpr operator pointer() noexcept { return m_data; }
		NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }
		NODISCARD constexpr auto data() noexcept -> pointer { return m_data; }
		NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }
		NODISCARD constexpr auto at(size_t const i) & noexcept -> reference { return m_data.at(i); }
		NODISCARD constexpr auto at(size_t const i) const & noexcept -> const_reference { return m_data.at(i); }
		NODISCARD constexpr auto at(size_t const i) && noexcept -> value_type && { return std::move(m_data.at(i)); }
		NODISCARD constexpr auto at(size_t const x, size_t const y) & noexcept -> reference { return at(y * _Width + x); }
		NODISCARD constexpr auto at(size_t const x, size_t const y) const & noexcept -> const_reference { return at(y * _Width + x); }
		NODISCARD constexpr auto at(size_t const x, size_t const y) && noexcept -> value_type && { return std::move(at(y * _Width + x)); }
		
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

		NODISCARD static constexpr self_type identity() noexcept {
			self_type temp{};
			for (size_t i = 0; i < size(); ++i) { temp[i] = (value_type)(i % _Width == i / _Height); }
			return temp;
		}

		NODISCARD static constexpr self_type fill(value_type value) noexcept {
			self_type temp{};
			for (auto & e : temp) { e = value; }
			return temp;
		}

		NODISCARD static constexpr self_type one() noexcept {
			self_type temp{};
			for (auto & e : temp) { e = (value_type)1; }
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, size_t W, size_t H
		> NODISCARD constexpr operator Matrix<T, W, H>() const noexcept
		{
			using Out = Matrix<T, W, H>;

			// same type
			if constexpr (std::is_same_v<Out, self_type>)
			{
				return (*this);
			}
			else
			{
				Out temp;

				// same dimensions
				if constexpr ((W == _Width) && (H == _Height))
				{
					for (size_t i = 0; i < (W * H); ++i)
					{
						temp[i] = static_cast<T>(m_data[i]);
					}
				}

				// different dimensions
				else
				{
					for (size_t i = 0; i < W * H; ++i)
					{
						if (size_t const x{ i % W }, y{ i / W }; (x < _Width) && (y < _Height))
						{
							temp[i] = static_cast<T>(m_data[y * _Width + x]);
						}
					}
				}

				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// equal
	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator==(Matrix<T1, W1, H1> const & a, Matrix<T2, W2, H2> const & b) noexcept
	{
		if constexpr (std::is_same_v<T1, T2>)
		{
			return (std::addressof(a) == std::addressof(b)) || util::range_equal(a.begin(), a.end(), b.begin(), b.end());
		}
		else
		{
			static_assert(std::is_convertible_v<decltype(b), decltype(a)>, "invalid comparison");
			return a == (decltype(a))b;
		}
	}

	// not equal
	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator!=(Matrix<T1, W1, H1> const & a, Matrix<T2, W2, H2> const & b) noexcept
	{
		if constexpr (std::is_same_v<T1, T2>)
		{
			return (std::addressof(a) != std::addressof(b)) && util::range_nequal(a.begin(), a.end(), b.begin(), b.end());
		}
		else
		{
			static_assert(std::is_convertible_v<decltype(b), decltype(a)>, "invalid comparison");
			return a != (decltype(a))b;
		}
	}

	// less
	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator<(Matrix<T1, W1, H1> const & a, Matrix<T2, W2, H2> const & b) noexcept
	{
		if constexpr (std::is_same_v<T1, T2>)
		{
			return (std::addressof(a) != std::addressof(b)) && util::range_less(a.begin(), a.end(), b.begin(), b.end());
		}
		else
		{
			static_assert(std::is_convertible_v<decltype(b), decltype(a)>, "invalid comparison");
			return a < (decltype(a))b;
		}
	}

	// greater
	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator>(Matrix<T1, W1, H1> const & a, Matrix<T2, W2, H2> const & b) noexcept
	{
		if constexpr (std::is_same_v<T1, T2>)
		{
			return (std::addressof(a) != std::addressof(b)) && util::range_greater(a.begin(), a.end(), b.begin(), b.end());
		}
		else
		{
			static_assert(std::is_convertible_v<decltype(b), decltype(a)>, "invalid comparison");
			return a > (decltype(a))b;
		}
	}

	// greater or equal
	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator<=(Matrix<T1, W1, H1> const & a, Matrix<T2, W2, H2> const & b) noexcept
	{
		if constexpr (std::is_same_v<T1, T2>)
		{
			return (std::addressof(a) == std::addressof(b)) || util::range_lequal(a.begin(), a.end(), b.begin(), b.end());
		}
		else
		{
			static_assert(std::is_convertible_v<decltype(b), decltype(a)>, "invalid comparison");
			return a <= (decltype(a))b;
		}
	}

	// less or equal
	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator>=(Matrix<T1, W1, H1> const & a, Matrix<T2, W2, H2> const & b) noexcept
	{
		if constexpr (std::is_same_v<T1, T2>)
		{
			return (std::addressof(a) == std::addressof(b)) || util::range_gequal(a.begin(), a.end(), b.begin(), b.end());
		}
		else
		{
			static_assert(std::is_convertible_v<decltype(b), decltype(a)>, "invalid comparison");
			return a >= (decltype(a))b;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Matrix<T, W, H> operator+(Matrix<T, W, H> const & a, B const & b) noexcept {
		auto c{ a };
		return c += b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Matrix<T, W, H> operator-(Matrix<T, W, H> const & a, B const & b) noexcept {
		auto c{ a };
		return c -= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Matrix<T, W, H> operator*(Matrix<T, W, H> const & a, B const & b) noexcept {
		auto c{ a };
		return c *= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Matrix<T, W, H> operator/(Matrix<T, W, H> const & a, B const & b) noexcept {
		auto c{ a };
		return c /= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Matrix<T, W, H> operator%(Matrix<T, W, H> const & a, B const & b) noexcept {
		auto c{ a };
		return c %= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Matrix<T, W, H> operator^(Matrix<T, W, H> const & a, B const & b) noexcept {
		auto c{ a };
		return c ^= b;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> NODISCARD constexpr auto operator-(Matrix<T, W, H> const & a) noexcept {
		return a * static_cast<T>(-1);
	}

	template <class T, size_t W, size_t H
	> NODISCARD constexpr auto operator+(Matrix<T, W, H> const & a) noexcept {
		return -(-(a));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/quaternion.hpp>

#include "vec2.inl"
#include "vec3.inl"
#include "vec4.inl"
#include "mat2.inl"
#include "mat3.inl"
#include "mat4.inl"

#endif // !_ISM_MATRIX_HPP_
